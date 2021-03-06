// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
//#include "ToonTanks/Pawns/PawnTank.h"

//Actor and pawns
#include  "ToonTanks/Actors/ProjectileBase.h"
#include  "ToonTanks/Components/HealthComponent.h"
#include  "ToonTanks/Actors/RescueZone.h"

//Engine
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

//Components
#include "Components/StaticMeshComponent.h"
//#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"



//Libraries
#include "Kismet/GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

#include "Components/AudioComponent.h"


APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SetupAttachment(GetTurretMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera-> SetupAttachment(SpringArm);
	
	
	
	//Other view
	TurretSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Turret Spring Arm"));	
	TurretSpringArm->SetupAttachment(GetCapsule());

	//TurretSpringArm->SetupAttachment(GetTurretMesh());
	//TurretSpringArm->Activate(false);

	TurretCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Turret Camera"));	
	TurretCamera->SetupAttachment(TurretSpringArm);
	TurretCamera->Activate(false);


	
	
}


// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	Temperatura = 0;
	PlayerControllerRef = Cast<APlayerController>(GetController());




	//Muy Importante Descomentar en la versi�n Shipped
	//Load(); //For Debug
	//Load();
}



void APawnTank::ChangeCameraView(bool bInBaseRoot)
{
	//default false 
	Camera->Activate(false);
	TurretCamera->Activate(false);

	

	if (bInBaseRoot) 
	{ 
		
		
		Camera->Activate(true);
		
	}
	else
	{
		
		
		TurretCamera->Activate(true);
		

	}

	LastLocation = GetActorForwardVector();
}


// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move();


	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	//TArray<>
	//Requiere Visibility
	
	//FHitResult TraceHitResult
	//PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	//FVector HitLocation = TraceHitResult.ImpactPoint;
	//FVector HitLocation = TraceHitResult.ImpactPoint;
	//RotateTurret(HitLocation);
	
	
	FHitResult Hit, FingerHit;
	PlayerControllerRef->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit);
	
	//ETouchIndex::Type FingerIndex::Touch3;
	//ECollisionChannel TraceChannel;

	//For Mobile
	PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Type::Touch3, ECC_Visibility,true, FingerHit);

	//PlayerControllerRef->GetHitResultUnderFinger(ETouchIndex::Type::Touch3, ECC_Visibility, true, FingerHit);
	if (Hit.bBlockingHit || FingerHit.bBlockingHit)
	{
		//RotateTurret(FingerHit.ImpactPoint);
		if (Hit.bBlockingHit) { RotateTurret(Hit.ImpactPoint); LastLocation =Hit.ImpactPoint;}
		if (FingerHit.bBlockingHit) { RotateTurret(FingerHit.ImpactPoint); LastLocation = FingerHit.ImpactPoint;}		
	}
	else
	{
		
		//RotateTurret(GetActorForwardVector());
		RotateTurret(LastLocation);
	}
	
	

	
	
	

}

void APawnTank::Rotate(float DeltaTime)
{
	CurrentYaw = GetTurretMesh()->GetComponentRotation().Yaw;


}

//
//void APawnTank::RotateTurret(FVector LookAtTarget)
//{
//	//update TurretMesh to face the LookAtTarget passed in from the child Class
//	// TurretMesh->SetWorldRotation()...
//	// Find Rotation value to look at. Rot Start  pos x e y del target y z de la torreta
//	 FVector StartLocation = GetTurretMesh()->GetComponentLocation();
//
//
//
//	
//	if ( ((LookAtTarget.X < 100.f) && (LookAtTarget.X > -100.f))  ||
//		 ((LookAtTarget.Y < 100.f) && (LookAtTarget.Y > -100.f))
//		)
//	{
//		    float Target_x = 0.f;
//			float Target_y = 0.f;
//			//Target_x = FMath::Clamp(LookAtTarget.X, -CameraMaxMovement, +10.f);
//
//			FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation,
//				//FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z));
//				FVector(Target_x, Target_y, GetTurretMesh()->GetComponentLocation().Z));
//
//			// Rotate Turret.
//			GetTurretMesh()->SetWorldRotation(TurretRotation);
//	}
//	else
//	{
//		Super::RotateTurret(LookAtTarget);
//	}
//
//	
//
//
//
//
//
//	
//	//TurretMesh->SetWorldRotation(TurretRotation,true,nullptr,ETeleportType::ResetPhysics);
//
//	//
//	
//	
//
//	
//
//}
//




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
	FHitResult Hit;
	AddActorLocalOffset(MoveDirection, true, &Hit);
	if (Hit.IsValidBlockingHit())
	{
		UE_LOG(LogTemp, Warning, TEXT("Algo ha bloqueado al PawnTank"));
		MoveDirection = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("MoveDirection: %f "), MoveDirection.Size());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nada Bloquea al PawnTank"));
	}
	//AddActorWorldOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
	//AddActorWorldRotation(RotationDirection, true);
}

void APawnTank::CoolDown()
{
	if (HotParticle) {

		

		UGameplayStatics::SpawnEmitterAtLocation(this, HotParticle, GetActorLocation() + FVector(0.f, 0.f, 100.f));
	}
	

	if (HotSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HotSound, GetActorLocation());
		
	}
	


}





void APawnTank::Fire()
{
	

	
	if (bIsCooldown)
	{
	 
		UE_LOG(LogTemp, Warning, TEXT("Arma sobrecalentada"));

		CoolDown();
		IsCoolDown();
		return;
	}


	Temperatura++;

	if (Temperatura >= TempMax)bIsCooldown = true;
	else bIsCooldown = false;

	UE_LOG(LogTemp, Error, TEXT("Temperatura: %f"), Temperatura);

	IsJustFiring();

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

		Temperatura+=2.f;
		

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
			
			Temperatura++;

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



float APawnTank::GetTemperatura() const
{
	return Temperatura;
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

