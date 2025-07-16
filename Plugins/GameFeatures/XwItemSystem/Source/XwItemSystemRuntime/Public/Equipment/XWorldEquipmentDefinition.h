// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SubclassOf.h"

#include "XWorldEquipmentDefinition.generated.h"

class AActor;
class ULyraAbilitySet;
class UXWorldEquipmentInstance;

USTRUCT()
struct FXWorldEquipmentActorToSpawn
{
	GENERATED_BODY()

	FXWorldEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};


/**
 * UXWorldEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UXWorldEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UXWorldEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UXWorldEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const ULyraAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FXWorldEquipmentActorToSpawn> ActorsToSpawn;
};
