// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "XWorldPickupDefinition.generated.h"

class UXWorldInventoryItemDefinition;
class UNiagaraSystem;
class UObject;
class USoundBase;
class UStaticMesh;

/**
 * 
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, Const, Meta = (DisplayName = "XWorld Pickup Data", ShortTooltip = "Data asset used to configure a pickup."))
class UXWorldPickupDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	//Defines the pickup's actors to spawn, abilities to grant, and tags to add
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup|Equipment")
	TSubclassOf<UXWorldInventoryItemDefinition> InventoryItemDefinition;

	//Visual representation of the pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	//Cool down time between pickups in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup")
	int32 SpawnCoolDownSeconds;

	//Sound to play when picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	//Sound to play when pickup is respawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	//Particle FX to play when picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup")
	TObjectPtr<UNiagaraSystem> PickedUpEffect;

	//Particle FX to play when pickup is respawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup")
	TObjectPtr<UNiagaraSystem> RespawnedEffect;
};


UCLASS(MinimalAPI, Blueprintable, BlueprintType, Const, Meta = (DisplayName = "XWorld Weapon Pickup Data", ShortTooltip = "Data asset used to configure a weapon pickup."))
class UXWorldWeaponPickupDefinition : public UXWorldPickupDefinition
{
	GENERATED_BODY()

public:

	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup|Mesh")
	FVector WeaponMeshOffset;

	//Sets the height of the display mesh above the Weapon spawner
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|Pickup|Mesh")
	FVector WeaponMeshScale = FVector(1.0f, 1.0f, 1.0f);
};
