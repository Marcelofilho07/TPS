// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TPSGameInstance.h"
#include "TPSMainGameUserWidget.generated.h"

/**
 * Lobby Widget
 */
UCLASS()
class TPS_API UTPSMainGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetRoundStartTime(float Time);

	void PassTime();

	void SetOnlinePlayersText(FString Player);

protected:
	virtual void NativeConstruct() override;

	UTPSGameInstance* GameInstance;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* RoundStartTimer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* OnlinePlayersText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonReady;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonCancel;

	UFUNCTION()
	void OnReadyButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
	
};
