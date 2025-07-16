// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/XWorldInventoryItemDefinition.h"

#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldInventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UXWorldInventoryItemDefinition

UXWorldInventoryItemDefinition::UXWorldInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UXWorldInventoryItemFragment* UXWorldInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UXWorldInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UXWorldInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UXWorldInventoryItemDefinition

const UXWorldInventoryItemFragment* UXWorldInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UXWorldInventoryItemDefinition> ItemDef, TSubclassOf<UXWorldInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UXWorldInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

