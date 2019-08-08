// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "Button.h"
#include "MainMenu.h"

bool UServerRow::Setup(UMainMenu * Parent, uint32 Index)
{
	if (!ensure(RowButton != nullptr)) return false;

	this->Parent = Parent;
	this->Index = Index;

	if (!ensure(this->Parent != nullptr))
		return false;

	RowButton->OnClicked.AddDynamic(this, &UServerRow::RowButtonPressed);
	return true;
}

void UServerRow::RowButtonPressed()
{
	Parent->SelectIndex(Index);
	//TODO: Select server and todo2 join that server.
}
