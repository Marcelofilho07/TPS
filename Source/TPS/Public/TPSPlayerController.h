// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPS/TPSCharacter.h"
#include "TPSPlayerController.generated.h"

/**
 * PlayerController. A lot of communication between client and server and HUD updating is handled here.
 */
UCLASS()
class TPS_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//Set ReadyState in lobby, Server RPC.
	UFUNCTION()
		void SetIsReadyState();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsReadyState();
	void Server_SetIsReadyState_Implementation();
	FORCEINLINE bool Server_SetIsReadyState_Validate() { return true; }

	//Server side to update the player list when a new player join the server. Server RPC. Calls UpdateHUDPlayerList to update clients.
	UFUNCTION()
	void UpdatePlayerList();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdatePlayerList();
	void Server_UpdatePlayerList_Implementation();
	FORCEINLINE bool Server_UpdatePlayerList_Validate() { return true; }

	void CreateHUD(); //In Game HUD

	//Update LobbyPlayerList to all clients. Client RPC.
	UFUNCTION(Client, Reliable, WithValidation)
	void UpdateHUDPlayerList(const FString& PlayerJoining);
	void UpdateHUDPlayerList_Implementation(const FString& PlayerJoining);
	FORCEINLINE bool UpdateHUDPlayerList_Validate(const FString& PlayerJoining) { return true; }

	//Create LobbyHUD to all clients. Client RPC
	UFUNCTION(Client, Reliable, WithValidation)
	void CreateLobbyHUD();
	void CreateLobbyHUD_Implementation();
	FORCEINLINE bool CreateLobbyHUD_Validate() { return true;  }

	//Collapse LobbyHUD to all clients. Client RPC
	UFUNCTION(Client, Reliable, WithValidation)
	void CollapseLobbyHUD();
	void CollapseLobbyHUD_Implementation();
	FORCEINLINE bool CollapseLobbyHUD_Validate() { return true; }

	//Start Round Start Countodown in LobbyHUD to all clients. Client RPC
	UFUNCTION(Client, Reliable, WithValidation)
	void StartCountdownLobbyHUD();
	void StartCountdownLobbyHUD_Implementation();
	FORCEINLINE bool StartCountdownLobbyHUD_Validate() { return true; }

	//Set the round winner and send to In Game HUD. Client RPC
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRoundWinner(const FString& RoundWinnerName, float Points);
	void SetRoundWinner_Implementation(const FString& RoundWinnerName, float Points);
	FORCEINLINE bool SetRoundWinner_Validate(const FString& RoundWinnerName, float Points) { return true; }

	class UTPSGameUIUserWidget* GetHUD();

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTPSGameUIUserWidget* UIHUDWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UTPSGameUIUserWidget> UIHUDWidgetClass;

	class UTPSMainGameUserWidget* GetLobbyHUD();

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTPSMainGameUserWidget* LobbyUIHUDWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UTPSMainGameUserWidget> LobbyUIHUDWidgetClass;

	//Set respawn countdown when player dies. Client RPC.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	FORCEINLINE bool SetRespawnCountdown_Validate(float RespawnTimeRemaining) { return true; }

protected:
	ATPSPlayerController();
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	class UTPSMenuUserWidget* UserInterface;

};
