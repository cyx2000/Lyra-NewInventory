// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "XWorldReticleWidgetBase.generated.h"

class UXWorldInventoryItemInstance;
class UXWorldWeaponInstance;
class UObject;
struct FFrame;

UCLASS(Abstract)
class UXWorldReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UXWorldReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UXWorldWeaponInstance* InWeapon);

	/** Returns the current weapon's diametrical spread angle, in degrees */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeSpreadAngle() const;

	/** Returns the current weapon's maximum spread radius in screenspace units (pixels) */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeMaxScreenspaceSpreadRadius() const;

	/**
	 * Returns true if the current weapon is at 'first shot accuracy'
	 * (the weapon allows it and it is at min spread)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFirstShotAccuracy() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UXWorldWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UXWorldInventoryItemInstance> InventoryInstance;
};
