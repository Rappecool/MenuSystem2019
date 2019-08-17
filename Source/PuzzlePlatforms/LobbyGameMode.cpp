// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "PuzzlePlatforms/PuzzlePlatformsGameInstance.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NumberOfPlayersInLobby++;
	if (NumberOfPlayersInLobby >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayersInLobby--;
}

int16 ALobbyGameMode::GetNumberOfPlayersInLobby() const
{
	return NumberOfPlayersInLobby;
}

void ALobbyGameMode::StartGame()
{ 
	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!GameInstance)
		return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;
	bUseSeamlessTravel = true;	//enables non blocking loading of map. So we don't have 2 large maps in memory at same time, we have loadingMap in between when using seamlesstravel.
	GetWorld()->ServerTravel("/Game/Maps/GameMap?listen");
}
