// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameGameInstance.h"


UTankGameGameInstance::UTankGameGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance Constructor"));
}

void UTankGameGameInstance::Init()
{
	Super::Init();
	LevelID = 0;
}

void UTankGameGameInstance::UpdateLevelID()
{
	++LevelID;
	//UE_LOG(LogTemp, Error, TEXT("GI UpdateLevelID LevelID: %d"), LevelID);
}


void UTankGameGameInstance::UpdateLevelID(int32 Delta)
{
	LevelID+=Delta;
	//UE_LOG(LogTemp, Error, TEXT("GI UpdateLevelID LevelID: %d"), LevelID);
}

//

void UTankGameGameInstance::UpdateScore(int32 Delta)
{
	GI_Score+=Delta;
	//UE_LOG(LogTemp, Error, TEXT("GI UpdateLevelID LevelID: %d"), LevelID);
}

int32 UTankGameGameInstance::GetMissingInActionRescued() const
{
	return MissinInActionRescued;
}

int32 UTankGameGameInstance::GetGI_Score() const
{
	return GI_Score;
}

int32 UTankGameGameInstance::GetLevelID() const
{
	return LevelID;
}

