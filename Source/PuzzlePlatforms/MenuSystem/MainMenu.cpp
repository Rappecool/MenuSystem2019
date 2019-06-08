// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting server..."));
}

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;
	
	if (!ensure(Host != nullptr)) return false;
	//Binds onClicked variable from Host to our HostServer function.
	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	return true;
}
