// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:

protected:
	IMenuInterface* MenuInterface;

public:
	//Sets Interface to instance wherever we call it from.
	void setMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	//Automatically handles when level is being killed/player leaving.
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)override;
};
