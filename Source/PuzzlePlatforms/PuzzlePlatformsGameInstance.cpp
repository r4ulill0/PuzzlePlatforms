// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    UE_LOG(LogTemp, Warning, TEXT("Constructor constructing game instance"));
}

void UPuzzlePlatformsGameInstance::Init() 
{
    UE_LOG(LogTemp, Warning, TEXT("Init FUNCTION constructing game instance"));
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
}