﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PawnTank.h"

#include "Components/SceneComponent.h"


#include "Engine/EngineTypes.h"

APawnTurret::APawnTurret()
{
	//RootComponent = GetCapsule();

}



void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	

	if (World)
	{
		World->GetTimerManager().SetTimer(
			FireRateTimerHandle, // Temporizador
			this, //En este objeto Puede ser un delegado, otro objeto
			&APawnTurret::CheckFireCondition,
			FireRate, //Disparo cada 2 segundos 
			true,     //Loop
			0.2f      //Delay
		);
	}

	

}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn)
	{
		//PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
		//if (!PlayerPawn)
		return;
	}
	if (!PlayerPawn->GetPlayerAlive() || ReturnDistanceToPlayer() > FireRange)
	{
		return;
	}

	RotateTurret(PlayerPawn->GetActorLocation());
}



void APawnTurret::CheckFireCondition()
{
	//if Player == Null || Is Dead Then BAIL!!
	UE_LOG(LogTemp, Warning, TEXT("Soy: %s y mi Firerate es: %f"), *GetName(), FireRate);
	
	

	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		UE_LOG(LogTemp, Error, TEXT("No Detecto a al palyerpawn in PawnTurret"));
		return;	
	}
	//UE_LOG(LogTemp, Warning, TEXT("Detectado playerpawn  %s "), *PlayerPawn->GetName());

	//if Player Player  IS in Ranged Then Fire!!
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		//Fire
		//UE_LOG(LogTemp, Warning, TEXT("FIRE!!"));
		Fire();
	}

}


float APawnTurret::ReturnDistanceToPlayer()
{
	// Check if we have a valid reference to the player.
	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		return 0.0f;
	}

	float Distance = (  PlayerPawn->GetActorLocation() - GetActorLocation()  ).Size();
	
	return Distance;

}




void APawnTurret::HandleDestruction()
{
	// Call base pawn class HandleDestruction to play effects.
	Super::HandleDestruction();
	// Hide AI. TODO 
	Destroy();
	//- Create new function to Do this. 

}



float APawnTurret::GetScoreValue()
{
	return ScoreValue;

}