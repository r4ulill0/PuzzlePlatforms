// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "ConnectionMenu.generated.h"
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UConnectionMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMenuInterface(IMenuInterface* Interface);
protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UFUNCTION()
	void OnHostClicked();

	IMenuInterface* InterfaceToMenu;
};
