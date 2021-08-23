// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "ConnectionMenu.generated.h"
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UConnectionMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinAddress;

	UPROPERTY(meta = (BindWidget))
	class UButton* Return;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinSubmenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainSubmenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IpAddressField;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void HandleJoin();
};
