// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSGameplayAbility.h"
#include "TPSGA_Ready.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSGA_Ready : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UTPSGA_Ready();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
