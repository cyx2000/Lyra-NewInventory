// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/XWorldInventoryItemDefinition.h"

#include "XWInventoryFragment_ReticleConfig.generated.h"

class UXWorldReticleWidgetBase;
class UObject;

UCLASS()
class UXWInventoryFragment_ReticleConfig : public UXWorldInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<UXWorldReticleWidgetBase>> ReticleWidgets;
};
