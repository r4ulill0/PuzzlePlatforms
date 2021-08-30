// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/ConnectionMenu.h"
#include "MenuSystem/MenuWidget.h"

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
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(TEXT("NULL"));
    if (OnlineSubsystem != nullptr) 
    {
        UE_LOG(LogTemp, Warning, TEXT("Found online subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
        auto SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid()) 
        {
            UE_LOG(LogTemp, Warning, TEXT("Found SessionInterface"));
        }
    }

}

void UPuzzlePlatformsGameInstance::Host() 
{
    if (Menu != nullptr)
    {
        Menu->Teardown();
    }

    UEngine* Engine = GetEngine();
    if (!ensure (Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure (World !=nullptr)) return;

    World->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address) 
{
    if (Menu != nullptr)
    {
        Menu->Teardown();
    }

    UEngine* Engine = GetEngine();
    if (!ensure (Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute, false);
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
