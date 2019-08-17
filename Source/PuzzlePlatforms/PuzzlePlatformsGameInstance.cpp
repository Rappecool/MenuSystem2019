// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "UnrealNames.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
const static FName Name = NAME_GameSession;

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTriggerBPClass(TEXT("/Game/PuzzlePlatforms/BP_PlatformTrigger"));
	if (!ensure(PlatformTriggerBPClass.Class != nullptr))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PlatformTriggerBPClass.Class->GetName());

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr))
	{
		return;
	}
	MenuClass = MenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr))
	{
		return;
	}
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	//Creates Menu widget, to use our MenuWidget_BP.
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr)) return;
	Menu->Setup();
	Menu->setMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	//Creates Menu widget, to use our MenuWidget_BP.
	UMenuWidget* InGameMenuWidget = CreateWidget<UMenuWidget>(this, InGameMenuClass); //perhaps should be created on Init before.

	if (!ensure(InGameMenuWidget != nullptr)) return;
	InGameMenuWidget->Setup();
	InGameMenuWidget->setMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameMenuTest()
{
	if (!ensure(InGameClass != nullptr)) return;
	//Creates Menu widget, to use our MenuWidget_BP.
	UMenuWidget* InGameTestMenu = CreateWidget<UMenuWidget>(this, InGameClass);

	if (!ensure(Menu != nullptr)) return;
	InGameTestMenu->Setup();
	InGameTestMenu->setMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Init()
{
	auto* SubSystem = IOnlineSubsystem::Get();
	if (!ensure(SubSystem != nullptr))
		return;

	UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *SubSystem->GetSubsystemName().ToString());

	SessionInterface = SubSystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
	}
}

void UPuzzlePlatformsGameInstance::Host(FName HostName)
{
	DesiredServerName = HostName.ToString();
	if (SessionInterface.IsValid() && Menu != nullptr)
	{
		auto ExistingSession = SessionInterface->GetNamedSession(Menu->GetHostName());

		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(Menu->GetHostName());
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() && !SessionSearch.IsValid())
		return;

	if (Menu != nullptr)
	{
		Menu->OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
	}
	SessionInterface->JoinSession(0,Menu->GetHostName(), SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
		return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session..."));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS: OnCreateSesssion unsuccessful."));
		return;
	}
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;

	if (Menu != nullptr)
	{
		Menu->OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), World);
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));
	//Hosts and listens so players can join.
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS: OnDestroySessionComplete unsuccessful."));
		return;
	}
	else
	{
		CreateSession();
		UE_LOG(LogTemp, Warning, TEXT("OSS: OnDestroySessionComplete."));
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServerNames;
			//Test data for UI.
		//ServerNames.Add("Test Server 1");
		//ServerNames.Add("Test Server 2");
		//ServerNames.Add("Test Server 3");


		for (const auto& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("OSS - Found Session: nameSessionsIDs is: %s ."), *SearchResult.GetSessionIdStr());
			FServerData ServerData;

			ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			ServerData.HostUserName = SearchResult.Session.OwningUserName;

			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				ServerData.Name = ServerName;
			}
			else
			{
				ServerData.Name = "Could not find name.";
			}
			ServerNames.Add(ServerData);
		}

		Menu->SetServerList(ServerNames);
		UE_LOG(LogTemp, Warning, TEXT("OSS: Finished OnFindSessionComplete."));
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
			return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get Connect String."));
		return;
	}

		//TODO move GetEngine() to own function.
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
		return;
	PlayerController->ClientTravel(Address, TRAVEL_Absolute);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Unsuccessful. Print other Enums and try to catch error."));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 100;
		SessionSettings.bShouldAdvertise = true; //enables Steam to find this among server lists.
		SessionSettings.bUsesPresence = true; //enables Steam to find this among server lists.

		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
		SessionInterface->CreateSession(0, Menu->GetHostName(), SessionSettings);
	}
}
