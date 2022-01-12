// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPS/TPS.h"
#include "TPSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTPSGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	ETPSAbilityInputID AbilityInputID = ETPSAbilityInputID::None;
};
