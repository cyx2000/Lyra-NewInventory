// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/LyraAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "XWorldEquipmentManagerComponent.generated.h"

#define UE_API XWITEMSYSTEMRUNTIME_API

class UActorComponent;
class ULyraAbilitySystemComponent;
class UXWorldEquipmentDefinition;
class UXWorldEquipmentInstance;
class UXWorldEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FXWorldEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FXWorldAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FXWorldAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FXWorldEquipmentList;
	friend UXWorldEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UXWorldEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UXWorldEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FLyraAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FXWorldEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FXWorldEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FXWorldEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FXWorldAppliedEquipmentEntry, FXWorldEquipmentList>(Entries, DeltaParms, *this);
	}

	UXWorldEquipmentInstance* AddEntry(TSubclassOf<UXWorldEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UXWorldEquipmentInstance* Instance);

private:
	ULyraAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UXWorldEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FXWorldAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FXWorldEquipmentList> : public TStructOpsTypeTraitsBase2<FXWorldEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages equipment applied to a pawn
 */
UCLASS(MinimalAPI, BlueprintType, Const)
class UXWorldEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UE_API UXWorldEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UE_API UXWorldEquipmentInstance* EquipItem(TSubclassOf<UXWorldEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UE_API void UnequipItem(UXWorldEquipmentInstance* ItemInstance);

	//~UObject interface
	UE_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	UE_API virtual void InitializeComponent() override;
	UE_API virtual void UninitializeComponent() override;
	UE_API virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UE_API UXWorldEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UXWorldEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	UE_API TArray<UXWorldEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UXWorldEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FXWorldEquipmentList EquipmentList;
};

#undef UE_API
