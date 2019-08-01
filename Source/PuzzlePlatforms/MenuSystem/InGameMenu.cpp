// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Button.h"

void UInGameMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;
	if (MenuInterface != nullptr)
	{
		OnLevelRemovedFromWorld(World->GetCurrentLevel(), World);
		MenuInterface->LoadMainMenu();
	}
}

void UInGameMenu::CancelPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("InGameMenu CancelPressed called!"));
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
	//UE_LOG(LogTemp, Warning, TEXT("CancelPressed"));
	//auto* World = GetWorld();
	//OnLevelRemovedFromWorld(World->GetCurrentLevel(), World);
}

bool UInGameMenu::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT("InGameMenu Init called!"));
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	UE_LOG(LogTemp, Warning, TEXT("InGameMenu Init success!"));
	return success;
}

