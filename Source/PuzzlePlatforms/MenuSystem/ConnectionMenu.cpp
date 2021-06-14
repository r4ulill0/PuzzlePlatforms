// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionMenu.h"
#include "Components/Button.h"

bool UConnectionMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    // TODO setup
    if (!ensure(Host != nullptr)) return false;
    Host->OnClicked.AddDynamic(this, &UConnectionMenu::OnHostClicked);

    return true;
}

void UConnectionMenu::OnHostClicked() 
{
    UE_LOG(LogTemp, Warning, TEXT("Host menu option clicked!!"));
}
