// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"


bool UPauseMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    if (Quit == nullptr) return false;
    Quit->OnClicked.AddDynamic( this, &UPauseMenu::OnQuitClicked);

    if (Cancel == nullptr) return false;
    Cancel->OnClicked.AddDynamic(this, &UPauseMenu::OnCancelClicked);

    return true;
}

void UPauseMenu::OnCancelClicked() 
{
    Teardown();
}

void UPauseMenu::OnQuitClicked() 
{
    if (InterfaceToMenu != nullptr)
    {
        Teardown();
        InterfaceToMenu->LoadMainMenu();
    }
}
