// Copyright Epic Games, Inc. All Rights Reserved.

#include "Equipment/XWorldEquipmentDefinition.h"
#include "Equipment/XWorldEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XWorldEquipmentDefinition)

UXWorldEquipmentDefinition::UXWorldEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UXWorldEquipmentInstance::StaticClass();
}

