// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"

#include "ToonTanks/Actors/ProjectileBase.h"


//Engine
//#include "Engine.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

//Components
#include "Components/StaticMeshComponent.h"
//#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"



#include  "ToonTanks/Components/HealthComponent.h"

//#include "ToonTanks/Actors/RescueZone.h"


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

	//Muy Importante Descomentar en la versión Shipped
	//Load(); //For Debug
	//Load();
}


// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move();


	//Requiere Visibility
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

void APawnTank::Fire()
{
	
	
	if (GetMissingInAction() >= GetMaxPassenger())
	{
		if (!ProjectileClassLv3)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... ProjectileClassLv3 is NONE"));
			return;
		}

		FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotator = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* ProjectileLv3 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClassLv3, ProjectileSpawnLocation, ProjectileSpawnRotator);
		UE_LOG(LogTemp, Warning, TEXT("Fire!!... ProjectileClassLv3 "));
		ProjectileLv3->SetOwner(this);

		return;
	}

	if ( GetMissingInAction() >= GetMaxPassenger() / 2)
	{

		if (!ProjectileClassLv2)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... ProjectileClassLv2 is NONE"));
			return;
		}
		
			FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator ProjectileSpawnRotator = ProjectileSpawnPoint->GetComponentRotation();
			

			AProjectileBase* ProjectileLv2 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClassLv2, ProjectileSpawnLocation, ProjectileSpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Fire!!... ProjectileClassLv2 "));
			ProjectileLv2->SetOwner(this);

			return;
		
	}

	
	Super::Fire();
	
	
}

int32 APawnTank::GetMissingInAction() const
{
	return MissingInActions;
}

void APawnTank::SetMissingInAction(int32 NewRescued)
{
	MissingInActions += NewRescued;

	//if(MissingInActions>= MaxPassenger) UE_LOG(LogTemp, Warning, TEXT("APawnTank SetMissingInAction  Passenger Complete:  %i"), MissingInActions);
	MissingInActions = FMath::Clamp(MissingInActions, 0, MaxPassenger);


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


bool APawnTank::GetIsInZoneRescue() const
{
	return bIsInZoneRescue;
}

void APawnTank::SetIsInZoneRescue()
{

	bIsInZoneRescue= true;	
	//GetCapsule()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));	

}


void APawnTank::ResetIsInZoneRescue()
{

	bIsInZoneRescue = false;

	//GetCapsule()->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

}

