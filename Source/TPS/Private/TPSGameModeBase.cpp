// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPS/TPSCharacter.h"
#include "TPSPlayerState.h"
#include "TPSPlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "TPSMainGameUserWidget.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	RespawnDelay = 5.0f;
	bReplicates = true;
	bRoundStarted = false;
	RoundTime = 60.f;
	RoundStartTime = 3.f;
}

void ATPSGameModeBase::HeroDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (Controller->GetPawn())
	{
		ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);

		Controller->UnPossess();
		Controller->Possess(SpectatorPawn);
	}
	ATPSPlayerController* PC = Cast<ATPSPlayerController>(Controller);
	if (PC)
		PC->SetRespawnCountdown(RespawnDelay);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &ATPSGameModeBase::RespawnHero, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
}

void ATPSGameModeBase::RespawnHero(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		AActor* PlayerStart = FindPlayerStart(Controller);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ATPSCharacter* Hero = GetWorld()->SpawnActor<ATPSCharacter>(HeroClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
}

void ATPSGameModeBase::StartRound()
{

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
		{
			if (ATPSPlayerState* PS = Cast<ATPSPlayerState>(PlayerActor->PlayerState))
			{
				if (!PS->GetIsReady() || PS->GetInGame())
				{
					return;
				}
			}
		}
	}

	if (GetNumPlayers() > 1)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerActor = Iterator->Get();
			if (PlayerActor)
			{
				StartCountdown(PlayerActor);
			}
		}

		FTimerHandle EndRoundTimerHandle;
		FTimerDelegate EndRoundDelegate;

		EndRoundDelegate = FTimerDelegate::CreateUObject(this, &ATPSGameModeBase::EndRound);
		GetWorldTimerManager().SetTimer(EndRoundTimerHandle, EndRoundDelegate, (RoundStartTime + RoundTime), false);
		bRoundStarted = true;
	}
}


void ATPSGameModeBase::StartCountdown(AController* Controller)
{
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(Controller))
	{
		PC->StartCountdownLobbyHUD();
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (Controller->GetPawn())
	{
		ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);

		Controller->UnPossess();
		Controller->Possess(SpectatorPawn);
	}

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &ATPSGameModeBase::SpawnHero, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RoundStartTime, false);
}

void ATPSGameModeBase::SpawnHero(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(Controller))
		{
			PC->CollapseLobbyHUD();
		}
		AActor* PlayerStart = FindPlayerStart(Controller);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ATPSCharacter* Hero = GetWorld()->SpawnActor<ATPSCharacter>(HeroClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
}


void ATPSGameModeBase::EndRound()
{
	float HighestScore = 0;
	FString WinnerName;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
		{
			if (ATPSPlayerState* PS = Cast<ATPSPlayerState>(PlayerActor->PlayerState))
			{
				if (PS->GetPoints() > HighestScore)
				{
					HighestScore = PS->GetPoints();
					WinnerName = PS->GetName();
				}
				else if (PS->GetPoints() == HighestScore)
				{
					WinnerName += " " + PS->GetName();
				}
			}
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
		{
			ATPSPlayerController* PC = Cast<ATPSPlayerController>(PlayerActor);
			if (PC)
			{
				PC->SetRoundWinner(WinnerName, HighestScore);
			}
		}
	}

}

void ATPSGameModeBase::UpdatePlayersList()
{
	FString PlayersList;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
		{
			PlayersList += PlayerActor->GetName() + "\n";
		}
	}


	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
		{
			ATPSPlayerController* PC = Cast<ATPSPlayerController>(PlayerActor);
			if (PC)
			{
				PC->UpdateHUDPlayerList(PlayersList);
			}
		}
	}
}

void ATPSGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if ((GetNumPlayers()-1) == 1 && bRoundStarted)
	{
		FTimerHandle EndRoundTimerHandle;
		FTimerDelegate EndRoundDelegate;

		EndRoundDelegate = FTimerDelegate::CreateUObject(this, &ATPSGameModeBase::EndRound);
		GetWorldTimerManager().SetTimer(EndRoundTimerHandle, EndRoundDelegate, 1.f, false);
	}

	FTimerHandle UpdateListTimerHandle;
	FTimerDelegate UpdateListDelegate;

	UpdateListDelegate = FTimerDelegate::CreateUObject(this, &ATPSGameModeBase::UpdatePlayersList);
	GetWorldTimerManager().SetTimer(UpdateListTimerHandle, UpdateListDelegate, 1.f, false);
}

void ATPSGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (GetNumPlayers() >= 4)
	{
		ErrorMessage = "Unable to join lobby. Player limit reached.";
	}
}
