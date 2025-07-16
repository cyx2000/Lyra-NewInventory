// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/IXWPickupable.h"

#include "GameFramework/Actor.h"
#include "Inventory/XWorldInventoryManagerComponent.h"
#include "UObject/ScriptInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IXWPickupable)

class UActorComponent;

UXWPickupableStatics::UXWPickupableStatics()
	: Super(FObjectInitializer::Get())
{
}

TScriptInterface<IXWPickupable> UXWPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IXWPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UXWPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IXWPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IXWPickupable>();
}

void UXWPickupableStatics::AddPickupToInventory(UXWorldInventoryManagerComponent* InventoryComponent, TScriptInterface<IXWPickupable> Pickup)
{
	if (InventoryComponent && Pickup)
	{
		const FXWInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for (const FXWPickupTemplate& Template : PickupInventory.Templates)
		{
			InventoryComponent->AddItemDefinition(Template.ItemDef, Template.StackCount);
		}

		for (const FXWPickupInstance& Instance : PickupInventory.Instances)
		{
			InventoryComponent->AddItemInstance(Instance.Item);
		}
	}
}
