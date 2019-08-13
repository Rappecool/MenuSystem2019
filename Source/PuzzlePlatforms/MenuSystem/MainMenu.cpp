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

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	ServerList->ClearChildren();

	uint32 i = 0;

	for (const FServerData& ServerData : ServerNames)
	{
		auto* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		Row->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),ServerData.CurrentPlayers, ServerData.MaxPlayers)));
		
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

FName UMainMenu::GetHostName() const
{
	if (!ensure(ServerHostName != nullptr))
		return "";

	FString ConvertedFText = ServerHostName->GetText().ToString();
	FName ConvertedFString = FName(*ConvertedFText);
	return ConvertedFString;
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr))
			return;
		
	if (ServerHostName != nullptr)
	{
	MenuInterface->Host(GetHostName());
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex: %d"), SelectedIndex.GetValue());
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

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(HostMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(HostMenu);
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
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(ExitButton != nullptr)) return false;
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitPressed);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from CancelJoinMenuButton to our OpenMainMenu function.
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(CancelHostMenuButton != nullptr)) return false;
	//Binds onClicked variable from CancelJoinMenuButton to our OpenMainMenu function.
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(ConfirmHostMenuButton != nullptr)) return false;
	//Binds onClicked variable from JoinButton to our OpenJoinMenu function.
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	return true;
}
