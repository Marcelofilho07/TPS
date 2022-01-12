// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGA_Ready.h"
#include "AbilitySystemComponent.h"
#include "TPSPlayerController.h"

UTPSGA_Ready::UTPSGA_Ready()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = ETPSAbilityInputID::Ready;
}

void UTPSGA_Ready::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(ActorInfo->PlayerController.Get()))
	{
		PC->SetIsReadyState();
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
