// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/XWorldInventoryItemInstance.h"

#include "Inventory/XWorldInventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldInventoryItemInstance)

class FLifetimeProperty;

UXWorldInventoryItemInstance::UXWorldInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXWorldInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
}

#if UE_WITH_IRIS
void UXWorldInventoryItemInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;

	// Build descriptors and allocate PropertyReplicationFragments for this object
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS

void UXWorldInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UXWorldInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UXWorldInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UXWorldInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UXWorldInventoryItemInstance::SetItemDef(TSubclassOf<UXWorldInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

const UXWorldInventoryItemFragment* UXWorldInventoryItemInstance::FindFragmentByClass(TSubclassOf<UXWorldInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UXWorldInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}


