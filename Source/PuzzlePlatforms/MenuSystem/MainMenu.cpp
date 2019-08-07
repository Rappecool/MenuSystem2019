// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "ServerRow.h"
#include "PuzzlePlatformsGameInstance.h"


UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr))
		return;

	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames)
	{
		auto* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		ServerList->AddChild(Row);
	}
}


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
		//if (!ensure(IPAdressField != nullptr))
		//	return;

		//const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join("");
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(JoinMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(MainMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitPressed()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	
	PlayerController->ConsoleCommand("quit");
}

bool UMainMenu::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("MainMenu Init called!"));
	bool success = Super::Initialize();
	if (!success) return false;
	
	if (!ensure(HostButton != nullptr)) return false;
	//Binds onClicked variable from HostButton to our HostServer function.
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(ExitButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitPressed);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from CancelJoinMenuButton to our OpenMainMenu function.
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}
