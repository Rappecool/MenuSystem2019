// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Button.h"
#include "Components/WidgetSwitcher.h"

void UInGameMenu::QuitGame()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	//MenuSwitcher->SetActiveWidget()
}

void UInGameMenu::CancelGameMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
}

bool UInGameMenu::Initalize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	//Binds onClicked variable from HostButton to our HostServer function.
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);

	if (!ensure(QuitButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelGameMenu);
	return success;
}

void UInGameMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{

}
