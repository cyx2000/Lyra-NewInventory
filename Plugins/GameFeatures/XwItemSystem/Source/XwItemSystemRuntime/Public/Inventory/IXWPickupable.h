// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"

#include "UObject/ObjectPtr.h"
#include "IXWPickupable.generated.h"

template <typename InterfaceType> class TScriptInterface;

class AActor;
class UXWorldInventoryItemDefinition;
class UXWorldInventoryItemInstance;
class UXWorldInventoryManagerComponent;
class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct FXWPickupTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 StackCount = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UXWorldInventoryItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct FXWPickupInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UXWorldInventoryItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FXWInventoryPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FXWPickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FXWPickupTemplate> Templates;
};

/**  */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UXWPickupable : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IXWPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FXWInventoryPickup GetPickupInventory() const = 0;
};

/**  */
UCLASS()
class UXWPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UXWPickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IXWPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UXWorldInventoryManagerComponent* InventoryComponent, TScriptInterface<IXWPickupable> Pickup);
};
