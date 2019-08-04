// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"

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
		//OBS: Difference between Patuzzi, Try copying github code, aka Create UserWidget. DONE, didn't work.
		//17/7 update, Init still not getting called.

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
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

	auto* SubSystem = IOnlineSubsystem::Get();

	if (!ensure(SubSystem != nullptr))
		return;

	UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *SubSystem->GetSubsystemName().ToString());
}

void UPuzzlePlatformsGameInstance::Host()
{
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
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString &IpAddress)
{
	if (Menu != nullptr)
	{
		Menu->OnLevelRemovedFromWorld(GetWorld()->GetCurrentLevel(), GetWorld());
	}

		//TODO move GetEngine() to own function.
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf( TEXT("Joining %s"), *IpAddress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
		return;
		
	PlayerController->ClientTravel(IpAddress, TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr))
		return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", TRAVEL_Absolute);
}
