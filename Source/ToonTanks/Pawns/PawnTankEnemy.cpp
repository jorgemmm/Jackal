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


	DistTooFar = 25 * DistToShoot;

}

void APawnTankEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APawnTankEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Tank Move Dist : %f"), ReturnDistanceToPlayer()); //Debug
	//FVector Translation = Velocity * 100 * DeltaTime; //Debug	

	float distToPlayer = ReturnDistanceToPlayer();

	if ((distToPlayer < 0)  || distToPlayer > DistTooFar || distToPlayer < DistToShoot)
	{
		return;
	}

	
	
	Translation = UKismetMathLibrary::GetForwardVector(

		
		RotateBase(PlayerPawn->GetActorLocation())
	);

	Move();

	return;
	
	//below no needed
	
	if (ReturnDistanceToPlayer() >= DistToShoot)
	{		
		//GetActorLocation() no funciona porque capsule  no tiene componente Location
		//debes coger el basemesh
		//Lo mismo con player Pawn da problemas en el tick para coger el actor location
		//UE_LOG(LogTemp, Error, TEXT("Enemy tank go From: %s to:  %s "),*GetBaseMesh()->GetComponentLocation().ToCompactString(), *PlayerPawn->GetBaseMesh()->GetComponentLocation().ToCompactString());
		//UE_LOG(LogTemp, Error, TEXT("To: %s "),
		//*PlayerPawn->GetActorLocation().ToString());*/
		//FHitResult Hit;
		//AddActorWorldOffset(Translation, true, &Hit);

		Move();
		 
		
		
	}
	

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
		return -1.f;
	}


	 Distance = ( PlayerPawn->GetActorLocation() - GetActorLocation() ).Size();
	
	 /*Debug*/
	 //const FString DistanceHUDFormat = FString::Printf(TEXT("Distance to player %s : %f"), *GetName(), Distance);
	 //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, DistanceHUDFormat);
	 //UE_LOG(LogTemp, Warning, TEXT("At APawnTankEnemy::ReturnDistanceToPlayer  Dist Enemy Tank: %s to Player: %f"), *GetName(), Distance);
     

	//Distance = FVector::DistSquaredXY(PlayerPawn->GetActorLocation(), GetBaseMesh()->GetComponentLocation());
	//Distance = (PlayerPawn->GetBaseMesh()->GetComponentLocation() - GetBaseMesh()->GetComponentLocation()).Size();

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



