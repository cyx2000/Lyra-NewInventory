// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/GameStateComponent.h"

#include "AimAssistTargetManagerComponent.generated.h"

#define UE_API SHOOTERCORERUNTIME_API

enum class ECommonInputType : uint8;

class APlayerController;
class UObject;
struct FAimAssistFilter;
struct FAimAssistOwnerViewData;
struct FAimAssistSettings;
struct FAimAssistTargetOptions;
struct FCollisionQueryParams;
struct FLyraAimAssistTarget;

/**
 * The Aim Assist Target Manager Component is used to gather all aim assist targets that are within
 * a given player's view. Targets must implement the IAimAssistTargetInterface and be on the
 * collision channel that is set in the ShooterCoreRuntimeSettings. 
 */
UCLASS(MinimalAPI, Blueprintable)
class UAimAssistTargetManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:

	/** Gets all visible active targets based on the given local player and their ViewTransform */
	UE_API void GetVisibleTargets(const FAimAssistFilter& Filter, const FAimAssistSettings& Settings, const FAimAssistOwnerViewData& OwnerData, const TArray<FLyraAimAssistTarget>& OldTargets, OUT TArray<FLyraAimAssistTarget>& OutNewTargets);

	/** Get a Player Controller's FOV scaled based on their current input type. */
	static UE_API float GetFOVScale(const APlayerController* PC, ECommonInputType InputType);

	/** Get the collision channel that should be used to find targets within the player's view. */
	UE_API ECollisionChannel GetAimAssistChannel() const;
	
protected:

	/**
	 * Returns true if the given target passes the filter based on the current player owner data.
	 * False if the given target should be excluded from aim assist calculations 
	 */
	UE_API bool DoesTargetPassFilter(const FAimAssistOwnerViewData& OwnerData, const FAimAssistFilter& Filter, const FAimAssistTargetOptions& Target, const float AcceptableRange) const;

	/** Determine if the given target is visible based on our current view data. */
	UE_API void DetermineTargetVisibility(FLyraAimAssistTarget& Target, const FAimAssistSettings& Settings, const FAimAssistFilter& Filter, const FAimAssistOwnerViewData& OwnerData);
	
	/** Setup CollisionQueryParams to ignore a set of actors based on filter settings. Such as Ignoring Requester or Instigator. */
	UE_API void InitTargetSelectionCollisionParams(FCollisionQueryParams& OutParams, const AActor& RequestedBy, const FAimAssistFilter& Filter) const;
};

#undef UE_API
