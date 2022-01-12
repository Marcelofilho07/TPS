// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameUIUserWidget.h"

void UTPSGameUIUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitGameButton)
	{
		SetExitButtonVisibiliy(false);
		ExitGameButton->OnClicked.AddDynamic(this, &UTPSGameUIUserWidget::OnExitButtonClicked);
	}
}

void UTPSGameUIUserWidget::OnExitButtonClicked()
{
	GetOwningPlayer()->ConsoleCommand("Quit");
}

void UTPSGameUIUserWidget::SetExitButtonVisibiliy(bool bVisibility)
{
	if(bVisibility)
		ExitGameButton->SetVisibility(ESlateVisibility::Visible);
	else
		ExitGameButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UTPSGameUIUserWidget::SetCurrentHealth(float CurrentHealth)
{
	FText HealthText = FText::FromString(FString::SanitizeFloat(CurrentHealth));
	Health->SetText(HealthText);
	HealthBar->SetPercent(CurrentHealth / 1000.f);
}

void UTPSGameUIUserWidget::SetCurrentDamage(float CurrentDamage)
{
	if (CurrentDamage <= -50)
		DoubleDamage->SetText(FText::FromString("DOUBLE DAMAGE!"));

	else
		DoubleDamage->SetText(FText::FromString(""));
}

void UTPSGameUIUserWidget::SetCurrentTime(float Time)
{
	FText TimeText = FText::FromString(FString::SanitizeFloat(Time));
	RoundTime->SetText(TimeText);

	FTimerHandle RoundTimerHandle;
	FTimerDelegate RoundDelegate;

	RoundDelegate = FTimerDelegate::CreateUObject(this, &UTPSGameUIUserWidget::PassTime);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, RoundDelegate, 1, true);
}

void UTPSGameUIUserWidget::PassTime()
{
	FText TimeText = FText::FromString(FString::SanitizeFloat((FCString::Atof(*RoundTime->GetText().ToString()) - 1.f)));
	RoundTime->SetText(TimeText);
}

void UTPSGameUIUserWidget::SetRespawnTime(float Time)
{
	FText TimeText = FText::FromString(FString::SanitizeFloat(Time));
	RespawnTime->SetText(TimeText);

	FTimerHandle RoundTimerHandle;
	FTimerDelegate RoundDelegate;

	RoundDelegate = FTimerDelegate::CreateUObject(this, &UTPSGameUIUserWidget::PassTimeRespawn);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, RoundDelegate, 1, false);
}

void UTPSGameUIUserWidget::PassTimeRespawn()
{
	FText TimeText = FText::FromString(FString::SanitizeFloat((FCString::Atof(*RespawnTime->GetText().ToString()) - 1.f)));
	RespawnTime->SetText(TimeText);
	if (FCString::Atof(*RespawnTime->GetText().ToString()) > 0)
	{
		FTimerHandle RoundTimerHandle;
		FTimerDelegate RoundDelegate;

		RoundDelegate = FTimerDelegate::CreateUObject(this, &UTPSGameUIUserWidget::PassTimeRespawn);
		GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, RoundDelegate, 1, false);
	}
	else
		RespawnTime->SetText(FText::FromString(""));
}

void UTPSGameUIUserWidget::SetRoundWinner(FString RoundWinnerName, float Points)
{
	FText WinnerText = FText::FromString("ROUND WINNER: " + RoundWinnerName + "\n POINTS: " + FString::SanitizeFloat(Points));
	RoundWinnerText->SetText(WinnerText);
}


