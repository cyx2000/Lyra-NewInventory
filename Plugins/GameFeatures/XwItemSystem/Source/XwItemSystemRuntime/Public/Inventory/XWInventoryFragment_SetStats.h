// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Inventory/XWorldInventoryItemDefinition.h"

#include "XWInventoryFragment_SetStats.generated.h"

class UXWorldInventoryItemInstance;
class UObject;
struct FGameplayTag;

UCLASS()
class UXWInventoryFragment_SetStats : public UXWorldInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UXWorldInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
