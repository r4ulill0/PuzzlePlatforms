// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "ConnectionMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 TotalPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UConnectionMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UConnectionMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostCreate;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinAddress;

	UPROPERTY(meta = (BindWidget))
	class UButton* Return;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMain;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostSubmenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinSubmenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainSubmenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ConnectionList;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameInput;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void HandleJoin();

	UFUNCTION()
	void OnQuitClicked();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
private:
	TSubclassOf<class UUserWidget> ConnectionCandidateClass;
};
