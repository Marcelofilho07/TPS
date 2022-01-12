// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAttributeSet.h"
#include "Net/UnrealNetwork.h"

UTPSAttributeSet::UTPSAttributeSet() {}

void UTPSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet, Points, COND_None, REPNOTIFY_Always);
}

void UTPSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, Health, OldHealth);
}

void UTPSAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, Damage, OldDamage);
}

void UTPSAttributeSet::OnRep_Points(const FGameplayAttributeData& OldPoints)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, Points, OldPoints);
}
