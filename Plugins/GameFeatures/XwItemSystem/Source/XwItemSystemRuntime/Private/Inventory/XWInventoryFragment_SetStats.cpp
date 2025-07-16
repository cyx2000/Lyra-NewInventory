// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/XWInventoryFragment_SetStats.h"

#include "Inventory/XWorldInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWInventoryFragment_SetStats)

void UXWInventoryFragment_SetStats::OnInstanceCreated(UXWorldInventoryItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

int32 UXWInventoryFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
