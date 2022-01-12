// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSMenuUserWidget.h"
#include "Kismet/GameplayStatics.h"


void UTPSMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UTPSGameInstance>(GetGameInstance());

	if(ButtonCreateSession)
		ButtonCreateSession->OnClicked.AddDynamic(this, &UTPSMenuUserWidget::OnCreateSessionButtonClicked); 
	if(ButtonJoinSession)
		ButtonJoinSession->OnClicked.AddDynamic(this, &UTPSMenuUserWidget::OnJoinSessionButtonClicked); 
	if (ButtonQuitGame)
		ButtonQuitGame->OnClicked.AddDynamic(this, &UTPSMenuUserWidget::OnQuitGameButtonClicked);
	
}

void UTPSMenuUserWidget::OnCreateSessionButtonClicked()
{
	if (GameInstance)
	{
		GameInstance->CreateServer();
	}
}

void UTPSMenuUserWidget::OnJoinSessionButtonClicked()
{
	if (GameInstance)
	{
		GameInstance->JoinServer();
	}
}

void UTPSMenuUserWidget::OnQuitGameButtonClicked()
{
	GetOwningPlayer()->ConsoleCommand("Quit");
}
