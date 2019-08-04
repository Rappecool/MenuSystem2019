// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"

#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
		UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);

		virtual void Init();

		UFUNCTION(BlueprintCallable)
			void LoadMenuWidget();

		UFUNCTION(BlueprintCallable)
			void InGameLoadMenu();

		UFUNCTION(BlueprintCallable)
			void InGameMenuTest();

			//Hosts game, enables ?listen so joining can occur. changes level through WorldTravel.
		UFUNCTION(Exec)
			void Host() override;
			
			//Attempts to join game hosted by given IpAdress, through clientTravel.
		UFUNCTION(EXEC)
			void Join(const FString &IpAddress) override;

		virtual void LoadMainMenu() override;
	
private:
		TSubclassOf<class UUserWidget> MenuClass;
		TSubclassOf<class UUserWidget> InGameMenuClass;
		TSubclassOf<class UUserWidget> InGameClass;

		TSharedPtr <class FOnlineSessionSearch> SessionSearch;

		class UMainMenu* Menu;
		class UInGameMenu* InGameMenu;

		IOnlineSessionPtr SessionInterface;

		void OnCreateSessionComplete(FName SessionName, bool Success);
		void OnDestroySessionComplete(FName SessionName, bool Success);
		void OnFindSessionComplete(bool Success);

		void CreateSession();

};
