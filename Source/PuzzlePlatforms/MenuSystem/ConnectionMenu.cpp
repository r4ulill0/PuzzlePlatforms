// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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

    if (!ensure(JoinAddress != nullptr)) return false;
    JoinAddress->OnClicked.AddDynamic(this, &UConnectionMenu::HandleJoin);

    if (!ensure(Quit != nullptr)) return false;
    Quit->OnClicked.AddDynamic(this, &UConnectionMenu::OnQuitClicked);

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

void UConnectionMenu::HandleJoin() 
{
    if (!ensure(IpAddressField  != nullptr)) return;

    if (InterfaceToMenu != nullptr)
    {
        InterfaceToMenu->Join(IpAddressField->GetText().ToString());
    }
}

void UConnectionMenu::OnQuitClicked() 
{
    if (InterfaceToMenu != nullptr)
    {
        InterfaceToMenu->Quit();
    }
}