// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSMainGameUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameModeBase.h"
#include "TPS/TPSCharacter.h"
#include "TPSPlayerState.h"
#include "TPSPlayerController.h"

void UTPSMainGameUserWidget::SetRoundStartTime(float Time)
{
	FText TimeText = FText::FromString(FString::SanitizeFloat(Time));
	RoundStartTimer->SetText(TimeText);

	FTimerHandle RoundTimerHandle;
	FTimerDelegate RoundDelegate;

	RoundDelegate = FTimerDelegate::CreateUObject(this, &UTPSMainGameUserWidget::PassTime);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, RoundDelegate, 1, true);
}
void UTPSMainGameUserWidget::PassTime()
{
	FText TimeText = FText::FromString(FString::SanitizeFloat((FCString::Atof(*RoundStartTimer->GetText().ToString()) - 1.f)));
	RoundStartTimer->SetText(TimeText);
}

void UTPSMainGameUserWidget::SetOnlinePlayersText(FString Player)
{
	FText PlayersText = FText::FromString(Player);

	OnlinePlayersText->SetText(PlayersText);
}

void UTPSMainGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UTPSGameInstance>(GetGameInstance());

	if (ButtonReady)
		ButtonReady->OnClicked.AddDynamic(this, &UTPSMainGameUserWidget::OnReadyButtonClicked); // Example click binding.
	if (ButtonCancel)
		ButtonCancel->OnClicked.AddDynamic(this, &UTPSMainGameUserWidget::OnCancelButtonClicked); // Example click binding.

}

void UTPSMainGameUserWidget::OnReadyButtonClicked()
{
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(GetOwningPlayer()))
	{
		PC->SetIsReadyState();
	}
}

void UTPSMainGameUserWidget::OnCancelButtonClicked()
{
	if (GameInstance)
	{
		GameInstance->LeaveServer();
	}
}