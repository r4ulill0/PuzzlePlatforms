// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectionCandidate.h"
#include "Components/Button.h"
#include "ConnectionMenu.h"

void UConnectionCandidate::Setup(class UConnectionMenu* InParent, uint32 InIndex) 
{
    Parent = InParent;
    Index = InIndex;
    if (!ensure(ClickableRow != nullptr)) return;
    ClickableRow->OnClicked.AddDynamic(this, &UConnectionCandidate::HandleRowClick);
}

void UConnectionCandidate::HandleRowClick() 
{
    Parent->SelectIndex(Index);
}
