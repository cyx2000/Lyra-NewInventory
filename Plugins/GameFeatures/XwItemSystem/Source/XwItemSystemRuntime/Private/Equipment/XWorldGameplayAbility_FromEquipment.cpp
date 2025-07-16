// Copyright Epic Games, Inc. All Rights Reserved.

#include "Equipment/XWorldGameplayAbility_FromEquipment.h"
#include "Equipment/XWorldEquipmentInstance.h"
#include "Inventory/XWorldInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldGameplayAbility_FromEquipment)

UXWorldGameplayAbility_FromEquipment::UXWorldGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UXWorldEquipmentInstance* UXWorldGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UXWorldEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UXWorldInventoryItemInstance* UXWorldGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UXWorldEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UXWorldInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UXWorldGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("XWorld", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
