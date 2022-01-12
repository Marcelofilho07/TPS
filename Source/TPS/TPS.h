// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETPSAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Fire,
	Jump,
	Ready
};