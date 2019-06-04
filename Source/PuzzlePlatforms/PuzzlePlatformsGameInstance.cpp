// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance constructor called."));
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance Init() called."));
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

	
}

void UPuzzlePlatformsGameInstance::Join(const FString &IpAddress)
{
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
