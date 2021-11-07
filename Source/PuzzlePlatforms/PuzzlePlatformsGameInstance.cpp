// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/ConnectionMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My session game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> ConnectionMenu(TEXT("/Game/MenuSystem/WBP_ConnectionMenu"));
    if (!ensure(ConnectionMenu.Class != nullptr)) return;
    MenuClass = ConnectionMenu.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/MenuSystem/WBP_PauseMenu"));
    if (!ensure(PauseMenuBPClass.Class != nullptr)) return;
    PauseMenuClass = PauseMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init() 
{
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem != nullptr) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Found online subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid()) 
        {
            UE_LOG(LogTemp, Warning, TEXT("Found SessionInterface"));
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

            SessionSearch = MakeShareable(new FOnlineSessionSearch());
            if (SessionSearch.IsValid())
            {
                SessionSearch->bIsLanQuery = true;
                // SessionSearch->QuerySettings This will be used for steam session search
                SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
            }
        }
    }

    if (GEngine != nullptr)
    {
        GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailure);
    }
}

void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    LoadMainMenu();
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName) 
{
    if (SessionInterface.IsValid())
    {
        FName FormattedServerName;
        if (ServerName.Len() > 0)
        {
            FormattedServerName = FName(*ServerName);
        }
        else
        {
            FormattedServerName = SESSION_NAME;
        }

        auto ExistingSession = SessionInterface->GetNamedSession(FormattedServerName);

        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession(FormattedServerName);
        }

    }
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) 
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create a session"));
        return;
    }
    if (Menu != nullptr)
    {
        Menu->Teardown();
    }

    UEngine* Engine = GetEngine();
    if (!ensure (Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure (World !=nullptr)) return;

    SessionInterface->CancelFindSessions();
    World->ServerTravel(TEXT("/Game/PuzzlePlatforms/maps/Lobby?listen"));
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) 
{
    if (Success)
    {
        CreateSession(SessionName);
    }
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success) 
{
    if (Success && SessionSearch.IsValid())
    {
        TArray<FServerData> ServerList;
        for (FOnlineSessionSearchResult& Result: SessionSearch->SearchResults)
        {
            FServerData Server;
            FString ServerName;
            if (Result.Session.SessionSettings.Get(TEXT("ServerName"), ServerName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Session server name found %s"), *ServerName);
                Server.Name = ServerName;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Session name not found"));
                Server.Name = Result.GetSessionIdStr();
            }
            Server.HostUsername = Result.Session.OwningUserName;
            Server.TotalPlayers = Result.Session.SessionSettings.NumPublicConnections;
            Server.CurrentPlayers = Server.TotalPlayers - Result.Session.NumOpenPublicConnections;
            ServerList.Add(Server);
            UE_LOG(LogTemp, Warning, TEXT("Found online session %s"), *Result.GetSessionIdStr());
        }
        Menu->SetServerList(ServerList);
    }
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) 
{
    if (!SessionInterface.IsValid()) return;
    UE_LOG(LogTemp, Warning, TEXT("Join %s completed"), *SessionName.ToString());
    if (Result == EOnJoinSessionCompleteResult::Type::Success) 
    {
        APlayerController* PlayerController = GetFirstLocalPlayerController();
        if (!ensure (PlayerController != nullptr)) return;

        FString Address;
        if (SessionInterface->GetResolvedConnectString(SessionName, Address)) 
        {
            UEngine* Engine = GetEngine();
            if (!ensure (Engine != nullptr)) return;

            Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

            PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute, false);
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Connection failed, F in the chat"));
        }
    }

}

void UPuzzlePlatformsGameInstance::CreateSession(FName FormattedServerName) 
{
    if (SessionInterface)
    {
        FOnlineSessionSettings SessionSettings;
        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            SessionSettings.bIsLANMatch = true;
        } 
        else 
        {
            SessionSettings.bIsLANMatch = false;
        }
        SessionSettings.bUsesPresence = true;
        SessionSettings.NumPublicConnections = 5;
        SessionSettings.bShouldAdvertise = true;

        SessionSettings.Set(TEXT("ServerName"), FormattedServerName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        SessionInterface->CreateSession(0, FormattedServerName, SessionSettings);
    }
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index) 
{
    if (!SessionInterface.IsValid()) return;
    if (!SessionSearch.IsValid()) return;

    if (Menu != nullptr)
    {
        Menu->Teardown();
    }

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
    
}

void UPuzzlePlatformsGameInstance::Quit() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure (PlayerController != nullptr)) return;
    
    PlayerController->ConsoleCommand("quit");
}

void UPuzzlePlatformsGameInstance::LoadMainMenu() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    PlayerController->ClientTravel(TEXT("/Game/MenuSystem/ConnectionMenu"), ETravelType::TRAVEL_Absolute, false);
    
}

void UPuzzlePlatformsGameInstance::RefreshServers() 
{
    if (SessionSearch.IsValid())
    {
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UPuzzlePlatformsGameInstance::LoadMenu() 
{
    if (!ensure (MenuClass !=nullptr)) return;
    Menu = CreateWidget<UConnectionMenu>(this, MenuClass);
    if (!ensure(Menu != nullptr)) return;

    Menu->SetMenuInterface(this);
    Menu->Setup();
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu() 
{
    if (!ensure (PauseMenuClass !=nullptr)) return;
    UMenuWidget* LocalMenu = CreateWidget<UMenuWidget>(this, PauseMenuClass);
    if (!ensure(LocalMenu != nullptr)) return;

    LocalMenu->SetMenuInterface(this);
    LocalMenu->Setup();
}

void UPuzzlePlatformsGameInstance::StartSession()
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->StartSession(SESSION_NAME);
    }
} 