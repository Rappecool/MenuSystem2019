// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	bUseSeamlessTravel = true;	//enables non blocking loading of map. So we don't have 2 large maps in memory at same time, we have loadingMap in between when using seamlesstravel.
	NumberOfPlayersInLobby++;
	if (NumberOfPlayersInLobby >= 3)
	{
		GetWorld()->ServerTravel("/Game/Maps/GameMap?listen");
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
