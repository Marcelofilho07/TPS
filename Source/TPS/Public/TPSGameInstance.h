// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TPSPlayerController.h"
#include "TPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTPSGameInstance();

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();

	UFUNCTION(BlueprintCallable)
	void LeaveServer();


protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSesssionComplete(FName ServerName, bool Succeeded);

	virtual void OnFindSessionsComplete(bool Succeeded);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	virtual void OnSessionFailure(const FUniqueNetId& UniqueNetId, ESessionFailure::Type Result);
};
