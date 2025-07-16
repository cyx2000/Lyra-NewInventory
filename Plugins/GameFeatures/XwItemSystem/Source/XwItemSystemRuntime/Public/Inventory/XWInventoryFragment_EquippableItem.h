// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/XWorldInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "XWInventoryFragment_EquippableItem.generated.h"

class UXWorldEquipmentDefinition;
class UObject;

UCLASS()
class UXWInventoryFragment_EquippableItem : public UXWorldInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=XWorld)
	TSubclassOf<UXWorldEquipmentDefinition> EquipmentDefinition;
};
