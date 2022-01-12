// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSGameplayAbility.h"
#include "TPSGA_Fire.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSGA_Fire : public UTPSGameplayAbility
{
	GENERATED_BODY()
			
public:
	UTPSGA_Fire();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> PointsAttributeEffect;
};
