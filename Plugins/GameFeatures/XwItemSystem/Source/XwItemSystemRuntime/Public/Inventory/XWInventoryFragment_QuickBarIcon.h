// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/XWorldInventoryItemDefinition.h"
#include "Styling/SlateBrush.h"

#include "XWInventoryFragment_QuickBarIcon.generated.h"

class UObject;

UCLASS()
class UXWInventoryFragment_QuickBarIcon : public UXWorldInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush AmmoBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayNameWhenEquipped;
};
