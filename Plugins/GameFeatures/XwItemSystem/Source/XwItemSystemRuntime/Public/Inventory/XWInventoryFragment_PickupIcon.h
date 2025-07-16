// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/XWorldInventoryItemDefinition.h"
#include "UObject/ObjectPtr.h"

#include "XWInventoryFragment_PickupIcon.generated.h"

class UObject;
class USkeletalMesh;

UCLASS()
class UXWInventoryFragment_PickupIcon : public UXWorldInventoryItemFragment
{
	GENERATED_BODY()

public:
	UXWInventoryFragment_PickupIcon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
};
