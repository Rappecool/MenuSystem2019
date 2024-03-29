// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
private:
	int16 NumberOfPlayersInLobby = 0;
	UPROPERTY(EditAnywhere)
	float Duration = 0;
	void StartGame();
	FTimerHandle GameStartTimer;

public:
	 void PostLogin(APlayerController* NewPlayer) override;
	 void Logout(AController* Exiting) override;
	 int16 GetNumberOfPlayersInLobby() const;
};
