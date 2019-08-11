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

	uint32 i = 0;

	for (const FString& ServerName : ServerNames)
	{
		auto* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr))
			return;
		
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex: %d"), SelectedIndex.GetValue());

		//if (!ensure(IPAdressField != nullptr))
		//	return;

		//const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex not set."));
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

void UMainMenu::UpdateChildren()
{
	if (ServerList == nullptr)
		return;

	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i); //OBS, Checks left-most statement first, "short-circuit".
		}
	}
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
