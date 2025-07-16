// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

#include "XWorldGameplayAbility_FromEquipment.generated.h"

class UXWorldEquipmentInstance;
class UXWorldInventoryItemInstance;

/**
 * UXWorldGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UXWorldGameplayAbility_FromEquipment : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:

	UXWorldGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="XWorld|Ability")
	UXWorldEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "XWorld|Ability")
	UXWorldInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
