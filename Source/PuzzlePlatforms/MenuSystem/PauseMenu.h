// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPauseMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Cancel;

	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnQuitClicked();
};
