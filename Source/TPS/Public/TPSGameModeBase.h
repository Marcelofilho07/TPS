// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameModeBase.generated.h"

/**
 * Here we handle most of our in game logic and also some login and logout logic.
 */
UCLASS()
class TPS_API ATPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATPSGameModeBase();

	float RespawnDelay;

	float RoundTime;

	float RoundStartTime;

	bool bRoundStarted;

	void HeroDied(AController* Controller); //Called when player dies. Sets spawn timer and TimerDelegate to call Respawn Hero

	void RespawnHero(AController* Controller); //Respawn player

	void StartCountdown(AController* Controller); //Start initial 3seconds countdown and call SpawnHero after this time.

	void SpawnHero(AController* Controller); //Called when game starts. Same as RespawnHero but CollapseLobbyHUD

	void StartRound(); //Check if all players are ready and start round if true

	void EndRound(); //End the round. Find player with most points.

	void UpdatePlayersList(); //Always called when a controller is possessed. Update Lobby Player List.

	virtual void Logout(AController* Exiting) override; //Always called before a controller is destroyed. Update player lists if on lobby, else handle if the host is alone in game and ends it. 

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override; //Always called when a player try to join the session, check for player limit.

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class ATPSCharacter> HeroClass;

	TArray<class ATPSPlayerController*> PlayersInLobby;
};
