// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "TPSGameUIUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSGameUIUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetCurrentHealth(float CurrentHealth);

	void SetCurrentDamage(float CurrentDamage);

	void SetCurrentTime(float Time);

	void PassTime();

	void SetRespawnTime(float Time);

	void PassTimeRespawn();

	void SetRoundWinner(FString RoundWinnerName, float Points);

	void SetExitButtonVisibiliy(bool bVisibility);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Health;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DoubleDamage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RoundTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RespawnTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RoundWinnerText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitGameButton;

	UFUNCTION()
	void OnExitButtonClicked();
};
