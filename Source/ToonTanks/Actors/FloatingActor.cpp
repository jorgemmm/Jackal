﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"

//Components
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetVisualMesh()) 
	{
		FVector NewLocation = GetActorLocation();
		FRotator NewRotation = GetActorRotation();
		float RunningTime = GetGameTimeSinceCreation();
		float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
		
		NewLocation.Z += DeltaHeight * MovementScale;       //Scale our height by a factor of 20
		
		float DeltaRotation = DeltaTime * MovementScale;    //Rotate by 20 degrees per second
		NewRotation.Yaw += DeltaRotation;
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}

}

