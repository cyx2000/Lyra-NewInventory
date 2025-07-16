// Copyright Epic Games, Inc. All Rights Reserved.

#include "Equipment/XWorldQuickBarComponent.h"

#include "Equipment/XWorldEquipmentDefinition.h"
#include "Equipment/XWorldEquipmentInstance.h"
#include "Equipment/XWorldEquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Inventory/XWInventoryFragment_EquippableItem.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldQuickBarComponent)

class FLifetimeProperty;
class UXWorldEquipmentDefinition;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_XWorld_QuickBar_Message_SlotsChanged, "XWorld.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_XWorld_QuickBar_Message_ActiveIndexChanged, "XWorld.QuickBar.Message.ActiveIndexChanged");

UXWorldQuickBarComponent::UXWorldQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UXWorldQuickBarComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UXWorldQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

void UXWorldQuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UXWorldQuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num()-1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UXWorldQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UXWorldInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UXWInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UXWInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UXWorldEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (UXWorldEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

void UXWorldQuickBarComponent::UnequipItemInSlot()
{
	if (UXWorldEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

UXWorldEquipmentManagerComponent* UXWorldQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UXWorldEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UXWorldQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

UXWorldInventoryItemInstance* UXWorldQuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 UXWorldQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (const TObjectPtr<UXWorldInventoryItemInstance>& ItemPtr : Slots)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

void UXWorldQuickBarComponent::AddItemToSlot(int32 SlotIndex, UXWorldInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

UXWorldInventoryItemInstance* UXWorldQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UXWorldInventoryItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void UXWorldQuickBarComponent::OnRep_Slots()
{
	FXWorldQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = Slots;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_XWorld_QuickBar_Message_SlotsChanged, Message);
}

void UXWorldQuickBarComponent::OnRep_ActiveSlotIndex()
{
	FXWorldQuickBarActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = ActiveSlotIndex;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_XWorld_QuickBar_Message_ActiveIndexChanged, Message);
}

