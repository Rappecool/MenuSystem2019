// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "PuzzlePlatformsGameInstance.h"

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr))
			return;
		
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAdressField != nullptr))
			return;

		const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join(Adress);
		UE_LOG(LogTemp, Warning, TEXT("JoinServer MainMenu called."));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(JoinMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(MainMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;
	
	if (!ensure(HostButton != nullptr)) return false;
	//Binds onClicked variable from HostButton to our HostServer function.
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from CancelJoinMenuButton to our OpenMainMenu function.
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}
