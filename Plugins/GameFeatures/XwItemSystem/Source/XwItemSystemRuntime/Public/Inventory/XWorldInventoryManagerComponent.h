// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "XWorldInventoryManagerComponent.generated.h"

#define UE_API XWITEMSYSTEMRUNTIME_API

class UXWorldInventoryItemDefinition;
class UXWorldInventoryItemInstance;
class UXWorldInventoryManagerComponent;
class UObject;
struct FFrame;
struct FXWorldInventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FXWorldInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UXWorldInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FXWorldInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FXWorldInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FXWorldInventoryList;
	friend UXWorldInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UXWorldInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FXWorldInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FXWorldInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FXWorldInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UXWorldInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FXWorldInventoryEntry, FXWorldInventoryList>(Entries, DeltaParms, *this);
	}

	UXWorldInventoryItemInstance* AddEntry(TSubclassOf<UXWorldInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UXWorldInventoryItemInstance* Instance);

	void RemoveEntry(UXWorldInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FXWorldInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UXWorldInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FXWorldInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FXWorldInventoryList> : public TStructOpsTypeTraitsBase2<FXWorldInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages an inventory
 */
UCLASS(MinimalAPI, BlueprintType)
class UXWorldInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UE_API UXWorldInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API bool CanAddItemDefinition(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API UXWorldInventoryItemInstance* AddItemDefinition(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API void AddItemInstance(UXWorldInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API void RemoveItemInstance(UXWorldInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	UE_API TArray<UXWorldInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UE_API UXWorldInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef) const;

	UE_API int32 GetTotalItemCountByDefinition(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef) const;
	UE_API bool ConsumeItemsByDefinition(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	UE_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	UE_API virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY(Replicated)
	FXWorldInventoryList InventoryList;
};

#undef UE_API
