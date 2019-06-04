// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
		UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);

		virtual void Init();

			//Hosts game, enables ?listen so joining can occur. changes level through WorldTravel.
		UFUNCTION(Exec)
			void Host();
			
			//Attempts to join game hosted by given IpAdress, through clientTravel.
		UFUNCTION(EXEC)
			void Join(const FString &IpAddress);
	
	
};
