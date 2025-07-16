// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "XWorldWeaponSpawner.generated.h"

#define UE_API XWITEMSYSTEMRUNTIME_API

namespace EEndPlayReason { enum Type : int; }

class APawn;
class UCapsuleComponent;
class UXWorldInventoryItemDefinition;
class UXWorldWeaponPickupDefinition;
class UObject;
class UPrimitiveComponent;
class UStaticMeshComponent;
struct FFrame;
struct FGameplayTag;
struct FHitResult;

UCLASS(MinimalAPI, Blueprintable,BlueprintType)
class AXWorldWeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UE_API AXWorldWeaponSpawner();

protected:
	// Called when the game starts or when spawned
	UE_API virtual void BeginPlay() override;
	UE_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	UE_API virtual void Tick(float DeltaTime) override;

	UE_API void OnConstruction(const FTransform& Transform) override;

protected:
	//Data asset used to configure a Weapon Spawner
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	TObjectPtr<UXWorldWeaponPickupDefinition> WeaponDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponAvailability, Category = "XWorld|WeaponPickup")
	bool bIsWeaponAvailable;

	//The amount of time between weapon pickup and weapon spawning in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	float CoolDownTime;

	//Delay between when the weapon is made available and when we check for a pawn standing in the spawner. Used to give the bIsWeaponAvailable OnRep time to fire and play FX. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	float CheckExistingOverlapDelay;

	//Used to drive weapon respawn time indicators 0-1
	UPROPERTY(BlueprintReadOnly, Transient, Category = "XWorld|WeaponPickup")
	float CoolDownPercentage;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> PadMesh;

	UPROPERTY(BlueprintReadOnly, Category = "XWorld|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "XWorld|WeaponPickup")
	float WeaponMeshRotationSpeed;

	FTimerHandle CoolDownTimerHandle;

	FTimerHandle CheckOverlapsDelayTimerHandle;

	UFUNCTION()
	UE_API void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	//Check for pawns standing on pad when the weapon is spawned. 
	UE_API void CheckForExistingOverlaps();

	UFUNCTION(BlueprintNativeEvent)
	UE_API void AttemptPickUpWeapon(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "XWorld|WeaponPickup")
	UE_API bool GiveWeapon(TSubclassOf<UXWorldInventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn);

	UE_API void StartCoolDown();

	UFUNCTION(BlueprintCallable, Category = "XWorld|WeaponPickup")
	UE_API void ResetCoolDown();

	UFUNCTION()
	UE_API void OnCoolDownTimerComplete();

	UE_API void SetWeaponPickupVisibility(bool bShouldBeVisible);

	UFUNCTION(BlueprintNativeEvent, Category = "XWorld|WeaponPickup")
	UE_API void PlayPickupEffects();

	UFUNCTION(BlueprintNativeEvent, Category = "XWorld|WeaponPickup")
	UE_API void PlayRespawnEffects();

	UFUNCTION()
	UE_API void OnRep_WeaponAvailability();

	/** Searches an item definition type for a matching stat and returns the value, or 0 if not found */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XWorld|WeaponPickup")
	static UE_API int32 GetDefaultStatFromItemDef(const TSubclassOf<UXWorldInventoryItemDefinition> WeaponItemClass, FGameplayTag StatTag);
};

#undef UE_API
