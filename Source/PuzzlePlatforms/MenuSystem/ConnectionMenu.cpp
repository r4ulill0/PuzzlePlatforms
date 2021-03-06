// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionMenu.h"
#include "ConnectionCandidate.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

UConnectionMenu::UConnectionMenu(const FObjectInitializer &ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> ConnectionCandidateFinder(TEXT("/Game/MenuSystem/WBP_ConnectionCandidate"));
    if (!ensure(ConnectionCandidateFinder.Class != nullptr)) return;
    ConnectionCandidateClass = ConnectionCandidateFinder.Class;
}

void UConnectionMenu::SetServerList(TArray<FServerData> ServerList) 
{
    UWorld* World = GetWorld();
    if (!ensure (World != nullptr)) return;

    ConnectionList->ClearChildren();
    uint32 Index = 0;
    for (FServerData& Server: ServerList)
    {
        if (!ensure (ConnectionCandidateClass != nullptr)) return;
        UE_LOG(LogTemp, Warning, TEXT("ConnectionCandidateClass correct"));
        UConnectionCandidate* ConnectionCandidate = CreateWidget<UConnectionCandidate>(World, ConnectionCandidateClass);
        ConnectionCandidate->ServerName->SetText(FText::FromString(Server.Name));
        ConnectionCandidate->HostName->SetText(FText::FromString(Server.HostUsername));
        FString FractionText = FString::Printf(TEXT("%d/%d "), Server.CurrentPlayers, Server.TotalPlayers);
        ConnectionCandidate->PlayerPopulation->SetText(FText::FromString(FractionText));
        ConnectionCandidate->Setup(this, Index);
        Index++;
        ConnectionList->AddChild(ConnectionCandidate);
    }
}

void UConnectionMenu::SelectIndex(uint32 Index) 
{
    SelectedIndex = Index;
    UpdateChildren();
}

void UConnectionMenu::UpdateChildren()
{
    for (int32 i = 0; i<ConnectionList->GetChildrenCount(); i++)
    {
        UConnectionCandidate* Row = Cast<UConnectionCandidate>(ConnectionList->GetChildAt(i));
        if (Row != nullptr)
        {
            Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
        }
    }
}
bool UConnectionMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;


    if (!ensure(HostCreate != nullptr)) return false;
    HostCreate->OnClicked.AddDynamic(this, &UConnectionMenu::OnHostClicked);

    if (!ensure(Host != nullptr)) return false;
    Host->OnClicked.AddDynamic(this, &UConnectionMenu::OpenHostMenu);

    if (!ensure(Join != nullptr)) return false;
    Join->OnClicked.AddDynamic(this, &UConnectionMenu::OpenJoinMenu);

    if (!ensure(Return != nullptr)) return false;
    Return->OnClicked.AddDynamic(this, &UConnectionMenu::OpenMainMenu);

    if (!ensure(ReturnToMain != nullptr)) return false;
    ReturnToMain->OnClicked.AddDynamic(this, &UConnectionMenu::OpenMainMenu);

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
        FString ServerName = ServerNameInput->GetText().ToString();
        UE_LOG(LogTemp, Warning, TEXT("The input text is %s"), *ServerName);
        InterfaceToMenu->Host(ServerName);
    }
}

void UConnectionMenu::OpenHostMenu()
{
    if (!ensure(Switcher != nullptr)) return;
    if (!ensure(HostSubmenu != nullptr)) return;
    Switcher->SetActiveWidget(HostSubmenu);
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
    
    if (InterfaceToMenu != nullptr) InterfaceToMenu->RefreshServers();
}

void UConnectionMenu::HandleJoin() 
{
    if (SelectedIndex.IsSet() && InterfaceToMenu != nullptr) {
        InterfaceToMenu->Join(SelectedIndex.GetValue());
    } else {
        UE_LOG(LogTemp, Warning, TEXT("There is no selected index"));
    }
}

void UConnectionMenu::OnQuitClicked() 
{
    if (InterfaceToMenu != nullptr)
    {
        InterfaceToMenu->Quit();
    }
}