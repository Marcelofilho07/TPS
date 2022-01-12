// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "TPSGameModeBase.h"
/*
Here we handle most connection related logic.
*/
UTPSGameInstance::UTPSGameInstance()
{}

void UTPSGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind some important delegates to handle networking logic
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTPSGameInstance::OnCreateSesssionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTPSGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTPSGameInstance::OnJoinSessionComplete);
			SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UTPSGameInstance::OnSessionFailure);
		}
	}
}

void UTPSGameInstance::OnCreateSesssionComplete(FName ServerName, bool Succeeded)
{
	if (Succeeded) //When session is created, travel to main game map.
	{
		GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UTPSGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	//Right after finding a session, make user join.
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, FName("New Session"), SearchResults[0]); //We use the first result, since it's a random search.
		}
	}
}

void UTPSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//Travel player to the map after joining
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		if(SessionInterface->GetResolvedConnectString(SessionName, JoinAddress))
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UTPSGameInstance::OnSessionFailure(const FUniqueNetId& UniqueNetId, ESessionFailure::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSessionFailure"));
}

void UTPSGameInstance::CreateServer()
{
	//Basic session setup with settings.
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 3;
	SessionSettings.BuildUniqueId = true;

	SessionInterface->CreateSession(0, FName("New Session"), SessionSettings);
	SessionInterface->StartSession(FName("New Session"));
}

void UTPSGameInstance::JoinServer()
{
	//Basic session search.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UTPSGameInstance::LeaveServer()
{
	//on leave server, we travel our client to the main menu, End Session, then Destroy Session.
	//Destroy session is what should be called when a user wants to leave a server. For clients it will leave the session and for hosts it will destroy the session.
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PController->ClientTravel("/Game/ThirdPersonCPP/Maps/TPSMainMenuLevel", ETravelType::TRAVEL_Absolute);
	}
	SessionInterface->EndSession(FName("New Session"));
	SessionInterface->DestroySession(FName("New Session"));
}
