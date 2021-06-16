// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

bool UConnectionMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;


    if (!ensure(Host != nullptr)) return false;
    Host->OnClicked.AddDynamic(this, &UConnectionMenu::OnHostClicked);

    if (!ensure(Join != nullptr)) return false;
    Join->OnClicked.AddDynamic(this, &UConnectionMenu::OpenJoinMenu);

    if (!ensure(Return != nullptr)) return false;
    Return->OnClicked.AddDynamic(this, &UConnectionMenu::OpenMainMenu);

    return true;
}

void UConnectionMenu::OnHostClicked() 
{
    if (InterfaceToMenu != nullptr)
    {
        InterfaceToMenu->Host();
    }
}

void UConnectionMenu::OpenJoinMenu() 
{
    if (!ensure(Switcher != nullptr)) return;
    if (!ensure(JoinSubmenu != nullptr)) return;
    Switcher->SetActiveWidget(JoinSubmenu);
}

void UConnectionMenu::OpenMainMenu() 
{
    if (!ensure(Switcher != nullptr)) return;
    if (!ensure(MainSubmenu != nullptr)) return;
    Switcher->SetActiveWidget(MainSubmenu);
}

void UConnectionMenu::SetMenuInterface(IMenuInterface* Interface) 
{
    this->InterfaceToMenu = Interface;
}

void UConnectionMenu::Setup() 
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if (!ensure (World !=nullptr)) return;
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(this->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PlayerController->SetInputMode(InputMode);

    PlayerController->bShowMouseCursor = true;
}

void UConnectionMenu::Teardown() 
{
    this->RemoveFromViewport();

    UWorld* World = GetWorld();
    if (!ensure (World !=nullptr)) return;
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure (PlayerController != nullptr)) return;

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    PlayerController->bShowMouseCursor = false;
}
