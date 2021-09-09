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
public:
	UConnectionMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit;

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
	class UPanelWidget* ConnectionList;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void HandleJoin();

	UFUNCTION()
	void OnQuitClicked();

	TOptional<uint32> SelectedIndex;
private:
	TSubclassOf<class UUserWidget> ConnectionCandidateClass;
};
