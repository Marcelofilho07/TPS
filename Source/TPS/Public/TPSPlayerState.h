// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "TPSPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UTPSAbilitySystemComponent;
class UTPSAttributeSet;
UCLASS()
class TPS_API ATPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATPSPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UTPSAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const; //check if player is alive

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetDamage() const;

	UFUNCTION(BlueprintCallable)
	float GetPoints() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsReady() const;

	UFUNCTION(BlueprintCallable)
	void ToggleReady(); //Toggle Player Ready state in lobby

	UFUNCTION(BlueprintCallable)
	bool GetInGame() const; //Check if player is in game

	UFUNCTION(BlueprintCallable)
	void ToggleInGame(); //Toggle player in game state

protected:

	UPROPERTY(Replicated)
	bool bIsReady;

	UPROPERTY(Replicated)
	bool bInGame;

	UPROPERTY()
	UTPSAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UTPSAttributeSet* AttributeSet;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle DamageChangedDelegateHandle;
	FDelegateHandle PointsChangedDelegateHandle;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void DamageChanged(const FOnAttributeChangeData& Data);
	virtual void PointsChanged(const FOnAttributeChangeData& Data);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
