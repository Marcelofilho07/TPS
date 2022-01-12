// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "AbilitySystemComponent.h"
#include "TPS/TPSCharacter.h"
#include "TPSMenuUserWidget.h"
#include "TPSPlayerState.h"
#include "TPSGameInstance.h"
#include "TPSGameModeBase.h"
#include "TPSGameUIUserWidget.h"
#include "TPSMainGameUserWidget.h"

void ATPSPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (IsLocalPlayerController())
	{
		UIHUDWidget->SetRespawnTime(RespawnTimeRemaining);
	}
}

ATPSPlayerController::ATPSPlayerController()
{
}

void ATPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
		if (!PS->GetInGame())
		{
			CreateLobbyHUD();
			if (LobbyUIHUDWidget)
			{
				UpdatePlayerList();
			}
		}
	}
}

void ATPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (PS)
	{
		if (!PS->GetInGame())
		{
			CreateLobbyHUD();
			if (LobbyUIHUDWidget)
			{
				UpdatePlayerList();
			}
		}
		else
			CreateHUD();
	}
}

void ATPSPlayerController::SetIsReadyState()
{
	if (GetLocalRole() != ROLE_Authority)
		Server_SetIsReadyState();

	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (PS)
	{
		PS->ToggleReady();
	}

	if (ATPSGameModeBase* GM = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->StartRound();
	}

}

void ATPSPlayerController::Server_SetIsReadyState_Implementation()
{
	SetIsReadyState();
}

void ATPSPlayerController::UpdatePlayerList()
{
	if (GetLocalRole() != ROLE_Authority)
		Server_UpdatePlayerList();

	if (ATPSGameModeBase* GM = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->UpdatePlayersList();
	}
}

void ATPSPlayerController::Server_UpdatePlayerList_Implementation()
{
	UpdatePlayerList();
}

void ATPSPlayerController::CreateHUD()
{
	if (UIHUDWidget)
	{
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UTPSGameUIUserWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetCurrentTime(60.f);
}

void ATPSPlayerController::UpdateHUDPlayerList_Implementation(const FString& PlayerJoining)
{
	if (IsLocalPlayerController())
	{
		LobbyUIHUDWidget->SetOnlinePlayersText(PlayerJoining);
	}
}


void ATPSPlayerController::CreateLobbyHUD_Implementation()
{
	if (LobbyUIHUDWidget)
	{
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();
	if (!PS)
	{
		return;
	}

	LobbyUIHUDWidget = CreateWidget<UTPSMainGameUserWidget>(this, LobbyUIHUDWidgetClass);
	LobbyUIHUDWidget->AddToViewport();
	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}

void ATPSPlayerController::CollapseLobbyHUD_Implementation()
{
	if(IsLocalPlayerController())
		LobbyUIHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ATPSPlayerController::StartCountdownLobbyHUD_Implementation()
{
	if (IsLocalPlayerController())
		LobbyUIHUDWidget->SetRoundStartTime(3.f);
}

void ATPSPlayerController::SetRoundWinner_Implementation(const FString& RoundWinnerName, float Points)
{
	if (IsLocalPlayerController())
	{
		UIHUDWidget->SetRoundWinner(RoundWinnerName, Points);
		UIHUDWidget->SetExitButtonVisibiliy(true);
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
}

UTPSGameUIUserWidget* ATPSPlayerController::GetHUD()
{
	return UIHUDWidget;
}

UTPSMainGameUserWidget* ATPSPlayerController::GetLobbyHUD()
{
	return LobbyUIHUDWidget;
}
