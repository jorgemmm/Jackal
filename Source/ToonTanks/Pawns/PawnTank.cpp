// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
//#include "Engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

#include  "ToonTanks/Components/HealthComponent.h"


APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	
	SpringArm->SetupAttachment(RootComponent);
	
	Camera= CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	
	
}


// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}


// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move();

	PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

	FVector HitLocation = TraceHitResult.ImpactPoint;

	//UE_LOG(LogTemp, Warning, TEXT("FIRE!! at hole to: %s"), *HitLocation.ToString());

	RotateTurret(HitLocation);


}


// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
	//PlayerInputComponent->BindAction("Turbo", IE_Pressed, &APawnTank::Turbo);

}

bool APawnTank::GetPlayerAlive()
{
	return bAlive;
}


void APawnTank::SetPlayerReAlive()
{
	bAlive=true;	

	// Show Player.
	SetActorHiddenInGame(false);

	//TODO - Create new function to Do this. 
	SetActorTickEnabled(true);
}





void APawnTank::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);

}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}



void APawnTank::Turbo()
{
	//if (turbo habilitado)
	{
		//Multiplica Move Speed Rotate
		//Toglle IsTurboEnable to false
		//Set timer To Enable Turbo 3 sg.


	}
	//else
	{ 
		
	}
	
	
}

void APawnTank::EnableTurbo()
{
	//Toggle IsTurboEnable To true

	//Clear Timer To EanbleTurbo
}

int32 APawnTank::GetMissingInAction() const
{
	return MissingInActions;
}

void APawnTank::SetMissingInAction(int32 NewRescued)
{
	MissingInActions += NewRescued;
}


int32 APawnTank::GetMaxPassenger() const
{
	return MaxPassenger;
}



void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
 
	bAlive = false;

	// Hide Player.
	SetActorHiddenInGame(true);

	//TODO - Create new function to Do this. 
	SetActorTickEnabled(false);

}



