// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
//#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"



void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
	if (SetPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
		//bShowMouseCursor = true;
	}
	else
	{
		GetPawn()->DisableInput(this);
		//bShowMouseCursor = false;
	}

	bShowMouseCursor = SetPlayerEnabled;

	bEnableMouseOverEvents = SetPlayerEnabled;

	bEnableClickEvents= SetPlayerEnabled;

	bEnableTouchOverEvents = SetPlayerEnabled;
}

