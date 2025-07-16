// Copyright Epic Games, Inc. All Rights Reserved.

#include "Equipment/XWorldEquipmentManagerComponent.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "Equipment/XWorldEquipmentDefinition.h"
#include "Equipment/XWorldEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldEquipmentManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

//////////////////////////////////////////////////////////////////////
// FXWorldAppliedEquipmentEntry

FString FXWorldAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FXWorldEquipmentList

void FXWorldEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FXWorldAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FXWorldEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FXWorldAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FXWorldEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

ULyraAbilitySystemComponent* FXWorldEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<ULyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UXWorldEquipmentInstance* FXWorldEquipmentList::AddEntry(TSubclassOf<UXWorldEquipmentDefinition> EquipmentDefinition)
{
	UXWorldEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UXWorldEquipmentDefinition* EquipmentCDO = GetDefault<UXWorldEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UXWorldEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UXWorldEquipmentInstance::StaticClass();
	}
	
	FXWorldAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UXWorldEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const ULyraAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FXWorldEquipmentList::RemoveEntry(UXWorldEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FXWorldAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UXWorldEquipmentManagerComponent

UXWorldEquipmentManagerComponent::UXWorldEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UXWorldEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UXWorldEquipmentInstance* UXWorldEquipmentManagerComponent::EquipItem(TSubclassOf<UXWorldEquipmentDefinition> EquipmentClass)
{
	UXWorldEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UXWorldEquipmentManagerComponent::UnequipItem(UXWorldEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UXWorldEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FXWorldAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UXWorldEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UXWorldEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UXWorldEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UXWorldEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FXWorldAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UXWorldEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UXWorldEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing XWorldEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FXWorldAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UXWorldEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UXWorldEquipmentInstance* UXWorldEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UXWorldEquipmentInstance> InstanceType)
{
	for (FXWorldAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UXWorldEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UXWorldEquipmentInstance*> UXWorldEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UXWorldEquipmentInstance> InstanceType) const
{
	TArray<UXWorldEquipmentInstance*> Results;
	for (const FXWorldAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UXWorldEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}


