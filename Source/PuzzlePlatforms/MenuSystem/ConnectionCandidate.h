// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectionCandidate.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UConnectionCandidate : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ServerName;
};
