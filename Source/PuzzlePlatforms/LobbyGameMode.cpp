// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Engine/World.h"
#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	NumberOfPlayersInLobby++;
	if (NumberOfPlayersInLobby >= 3)
	{
	UE_LOG(LogTemp, Warning, TEXT("NumberOfPlayers >= 3"));
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	NumberOfPlayersInLobby--;
}

int16 ALobbyGameMode::GetNumberOfPlayersInLobby() const
{
	return NumberOfPlayersInLobby;
}
