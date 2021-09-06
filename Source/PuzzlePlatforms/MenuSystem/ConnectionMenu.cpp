// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionMenu.h"
#include "ConnectionCandidate.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


UConnectionMenu::UConnectionMenu(const FObjectInitializer &ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> ConnectionCandidateFinder(TEXT("/Game/MenuSystem/WBP_ConnectionCandidate"));
    if (!ensure(ConnectionCandidateFinder.Class != nullptr)) return;
    ConnectionCandidateClass = ConnectionCandidateFinder.Class;
}

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
    UE_LOG(LogTemp, Warning, TEXT("Handling join"));
    if (InterfaceToMenu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Interface correct"));
        // DELETE THIS
        // This will be only temp code to test scrollBox
        UWorld* World = this->GetWorld();
        if (!ensure (World != nullptr)) return;
        UE_LOG(LogTemp, Warning, TEXT("World correct"));

        if (!ensure (ConnectionCandidateClass != nullptr)) return;
        UE_LOG(LogTemp, Warning, TEXT("ConnectionCandidateClass correct"));
        UConnectionCandidate* ConnectionCandidate = CreateWidget<UConnectionCandidate>(World, ConnectionCandidateClass);
        ConnectionList->AddChild(ConnectionCandidate);
    // Keep this to stablish the connection
    //     InterfaceToMenu->Join(IpAddressField->GetText().ToString());
    }
}

void UConnectionMenu::OnQuitClicked() 
{
    if (InterfaceToMenu != nullptr)
    {
        InterfaceToMenu->Quit();
    }
}