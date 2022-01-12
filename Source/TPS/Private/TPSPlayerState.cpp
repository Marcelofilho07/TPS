// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerState.h"
#include "TPSAttributeSet.h"
#include "TPSAbilitySystemComponent.h"
#include "TPS/TPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "TPSPlayerController.h"
#include "TPSGameUIUserWidget.h"


ATPSPlayerState::ATPSPlayerState()
{
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTPSAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;

	bIsReady = false;
	bInGame = false;
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UTPSAttributeSet* ATPSPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

bool ATPSPlayerState::IsAlive() const
{
    return GetHealth() > 0.0f;
}

float ATPSPlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float ATPSPlayerState::GetDamage() const
{
    return AttributeSet->GetDamage();
}

float ATPSPlayerState::GetPoints() const
{
	return AttributeSet->GetPoints();
}

void ATPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ATPSPlayerState::HealthChanged);
		DamageChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetDamageAttribute()).AddUObject(this, &ATPSPlayerState::DamageChanged);
		PointsChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetPointsAttribute()).AddUObject(this, &ATPSPlayerState::PointsChanged);

	}
}

void ATPSPlayerState::HealthChanged(const FOnAttributeChangeData& Data) //Check if player died and change life bar.
{
	float Health = Data.NewValue;

	if (!IsAlive())
	{
		ATPSCharacter* TPSC = Cast<ATPSCharacter>(GetPawn());
		if (TPSC)
			TPSC->Die();
	}

	ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetOwner());
	if (PC)
	{
		UTPSGameUIUserWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetCurrentHealth(Health);
		}
	}
}

void ATPSPlayerState::DamageChanged(const FOnAttributeChangeData& Data) //Trigger double damage notification on screen
{
	float Damage = Data.NewValue;

	ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetOwner());
	if (PC)
	{
		UTPSGameUIUserWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetCurrentDamage(Damage);
		}
	}
}

void ATPSPlayerState::PointsChanged(const FOnAttributeChangeData& Data)
{
	float Points = Data.NewValue;
}

bool ATPSPlayerState::GetIsReady() const
{
	return bIsReady;
}

void ATPSPlayerState::ToggleReady()
{
	bIsReady = !bIsReady;
}

bool ATPSPlayerState::GetInGame() const
{
	return bInGame;
}

void ATPSPlayerState::ToggleInGame()
{
	bInGame = true;
}

void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ATPSPlayerState, bIsReady, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ATPSPlayerState, bInGame, COND_None, REPNOTIFY_Always);
}
