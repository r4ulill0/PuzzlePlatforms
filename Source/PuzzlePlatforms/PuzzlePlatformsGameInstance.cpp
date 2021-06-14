// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/ConnectionMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> ConnectionMenu(TEXT("/Game/MenuSystem/WBP_ConnectionMenu"));
    if (!ensure(ConnectionMenu.Class != nullptr)) return;
    MenuClass = ConnectionMenu.Class;
}

void UPuzzlePlatformsGameInstance::Init() 
{
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host() 
{

    UEngine* Engine = GetEngine();
    if (!ensure (Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure (World !=nullptr)) return;

    World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::JoinSession(const FString& Address) 
{
    UEngine* Engine = GetEngine();
    if (!ensure (Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2.5f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute, false);
}

void UPuzzlePlatformsGameInstance::LoadMenu() 
{
    if (!ensure (MenuClass !=nullptr)) return;
    UConnectionMenu* Menu = CreateWidget<UConnectionMenu>(this, MenuClass);
    
    if (!ensure (Menu !=nullptr)) return;
    Menu->AddToViewport();

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetWidgetToFocus(Menu->TakeWidget());
    PlayerController->SetInputMode(InputMode);

    PlayerController->bShowMouseCursor = true;

    Menu->SetMenuInterface(this);
}
