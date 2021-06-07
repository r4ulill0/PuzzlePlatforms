// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform() 
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay() 
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }
    GlobalStartLocation = GetActorLocation();
    GlobalEndLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if (ActiveTriggers > 0) 
    {
        if (HasAuthority())
        {
            FVector Location = GetActorLocation();
            float JourneyLength = (GlobalEndLocation - GlobalStartLocation).Size();
            float CurrentLength = (Location - GlobalStartLocation).Size();
            if (CurrentLength >= JourneyLength)
            {
                FVector Temp = GlobalStartLocation;
                GlobalStartLocation = GlobalEndLocation;
                GlobalEndLocation = Temp;
            }
            FVector Direction = (GlobalEndLocation - GlobalStartLocation).GetSafeNormal();
            Location += Direction * Speed * DeltaTime;
            SetActorLocation(Location);
        }
    }

}

void AMovingPlatform::AddActiveTrigger() 
{
    ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger() 
{
    if (ActiveTriggers > 0)
    {
        ActiveTriggers--;
    }
}
