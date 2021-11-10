// Víctor J. Pasín Rguez All Rights Reserved



#include "PawnTankEnemy.h"
//#include "ToonTanks/Pawns/PawnTankEnemy.h"

#include "ToonTanks/Pawns/PawnTank.h"

//Engine
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

//#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"


//Components
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "Components/BoxComponent.h"

//Utils
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//AI
//#include "Blueprint/AIBlueprintHelperLibrary.h"

#define OUT

APawnTankEnemy::APawnTankEnemy()
{
	PrimaryActorTick.bCanEverTick = true;


	

}

void APawnTankEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APawnTankEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (!PlayerPawn) {
		UE_LOG(LogTemp, Error, TEXT("Player Pawn Not Loaded"));
		return;

	}*/
	

	if ( !PlayerPawn || !PlayerPawn->GetPlayerAlive() ) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No Detecto a al palyerpawn in PawnTurret"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy Tank Move Dist : %f"), ReturnDistanceToPlayer());

	if (ReturnDistanceToPlayer() < DistToShoot)
	{
		return;
	}

	
	//FVector Translation = Velocity * 100 * DeltaTime;
	
	
	Translation = UKismetMathLibrary::GetForwardVector(

		
		RotateBase(PlayerPawn->GetActorLocation())
	);

	
	

	//if  (CheckDistance(PlayerPawn))	
	if (ReturnDistanceToPlayer() >= DistToShoot)
	{		
		//GetActorLocation() no funciona porque capsule no tiene componente Location
		// debes coger el basemesh
		//Lo mismo con player Pawn da problemas en el tick para coger el actor location
		//UE_LOG(LogTemp, Error, TEXT("Enemy tank go From: %s to:  %s "),*GetBaseMesh()->GetComponentLocation().ToCompactString(), *PlayerPawn->GetBaseMesh()->GetComponentLocation().ToCompactString());
				
		/*UE_LOG(LogTemp, Error, TEXT("To: %s "),
			*PlayerPawn->GetActorLocation().ToString());*/
		
		FHitResult Hit;
		
			
		
		Move();  //AddActorWorldOffset(Translation, true, &Hit);
		 
		
		
	}
	

}


bool APawnTankEnemy::CheckDistance(APawn* PawnTank)
{
	bool GoGoMove = false;
	
	if (ReturnDistanceToPlayer() >= DistToShoot)
	{
		//Demasiado lejos para disparar => Nos movemos
		//GetBaseMesh()->AddWorldOffset(Translation);
		return true;
	}
	else
	{
		//Distancia de disparo  Paramos
		return false;
	}
	
	//return GoGoMove;

}



void APawnTankEnemy::HandleDestruction()
{
	// Call base pawn class HandleDestruction to play effects.
	//Super::HandleDestruction();
	// Hide AI. TODO 
	//Destroy() is in Super::HandleDestruction();
	//- Create new function to Do this. 

	//Put another Particle effect
	
	if(DeathParticle)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetBaseMesh()->GetComponentLocation());
	else 
		UE_LOG(LogTemp, Error, TEXT("Enemy Tank DeathParticle not Herency")); 
	
	if(DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetBaseMesh()->GetComponentLocation());
	else	
		UE_LOG(LogTemp, Error, TEXT("Enemy Tank DeathSound not Herency"));

	Destroy();

}


float APawnTankEnemy::ReturnDistanceToPlayer()
{
	float Distance = Super::ReturnDistanceToPlayer();
	// Check if we have a valid reference to the player.
	


	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		UE_LOG(LogTemp, Error, TEXT("APawnTankEnemy.CPP ReturnDistanceToPlayer PlayerControllerRef is nullptr"));
		return 0.0f;
	}


	//Distance = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
	//Distance = FVector::DistSquaredXY(PlayerPawn->GetActorLocation(), GetBaseMesh()->GetComponentLocation());

	Distance = (PlayerPawn->GetBaseMesh()->GetComponentLocation() - GetBaseMesh()->GetComponentLocation()).Size();

	return Distance;

	
}


void APawnTankEnemy::Move()
{
	FHitResult Hit;
	AddActorWorldOffset(Translation, true, &Hit);

}

FRotator APawnTankEnemy::RotateBase(FVector LookAtTarget)
{

	FVector StartLocation = GetBaseMesh()->GetComponentLocation();
	
	FRotator BaseMeshRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation,
		FVector(LookAtTarget.X, LookAtTarget.Y, GetBaseMesh()->GetComponentLocation().Z));

	// Rotate Turret.

	GetBaseMesh()->SetWorldRotation(BaseMeshRotation);

	return BaseMeshRotation;
}



