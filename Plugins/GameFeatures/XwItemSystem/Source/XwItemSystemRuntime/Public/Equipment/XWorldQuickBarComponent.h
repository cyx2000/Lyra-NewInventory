// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"
#include "Inventory/XWorldInventoryItemInstance.h"

#include "XWorldQuickBarComponent.generated.h"

class AActor;
class UXWorldEquipmentInstance;
class UXWorldEquipmentManagerComponent;
class UObject;
struct FFrame;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UXWorldQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UXWorldQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="XWorld")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category="XWorld")
	void CycleActiveSlotBackward();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="XWorld")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<UXWorldInventoryItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UXWorldInventoryItemInstance* GetActiveSlotItem() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(int32 SlotIndex, UXWorldInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UXWorldInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	virtual void BeginPlay() override;

private:
	void UnequipItemInSlot();
	void EquipItemInSlot();

	UXWorldEquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY()
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UXWorldInventoryItemInstance>> Slots;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UXWorldEquipmentInstance> EquippedItem;
};


USTRUCT(BlueprintType)
struct FXWorldQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UXWorldInventoryItemInstance>> Slots;
};


USTRUCT(BlueprintType)
struct FXWorldQuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 ActiveIndex = 0;
};
