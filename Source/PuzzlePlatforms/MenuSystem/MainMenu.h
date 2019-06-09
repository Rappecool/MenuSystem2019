// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
	private:
		//Creates and binds button automatically to our BP. OBS needs to have the exact same name.
	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UFUNCTION()
	void HostServer();

	IMenuInterface* MenuInterface;

protected:
	virtual bool Initialize();

public:
	//Sets Interface to instance wherever we call it from.
	void setMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	//Automatically handles when level is being killed/player leaving.
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)override;

};
