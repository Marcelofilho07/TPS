// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TPSGameInstance.h"
#include "TPSMenuUserWidget.generated.h"

/**
 * Main Menu Widget. Calls Game Instance to handle networking logic.
 */
UCLASS(Abstract)
class TPS_API UTPSMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UTPSGameInstance* GameInstance;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonCreateSession;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonJoinSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonQuitGame;

	UFUNCTION()
	void OnCreateSessionButtonClicked();

	UFUNCTION()
	void OnJoinSessionButtonClicked();

	UFUNCTION()
	void OnQuitGameButtonClicked();
};
