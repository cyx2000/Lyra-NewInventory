// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapons/XWorldWeaponDebugSettings.h"
#include "Misc/App.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldWeaponDebugSettings)

UXWorldWeaponDebugSettings::UXWorldWeaponDebugSettings()
{
}

FName UXWorldWeaponDebugSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

