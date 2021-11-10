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
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"



//Libraries
#include "Kismet/GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

#include "Components/AudioComponent.h"


APawnTank::APawnTank()
{

	

	//RootComponent = GetCapsule();
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));	
	SpringArm->SetupAttachment(RootComponent);

	//SpringArm->SetupAttachment(GetCapsule());
	//SpringArm->SetupAttachment(GetTurretMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera-> SetupAttachment(SpringArm);
	
	
	
	//Other view
	TurretSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Turret Spring Arm"));	
	TurretSpringArm->SetupAttachment(RootComponent);

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


	LastLocation = GetActorForwardVector();

	//Muy Importante Descomentar en la versi�n Shipped
	//Load(); //For Debug
	//Load();
}



// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PlayerControllerRef = Cast<APlayerController>(GetController());
	if (!PlayerControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("PawnTank.CPP tick PlayerControllerRef is nullptr"));
		return;
	}
		

	Rotate();
	Move();


	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	//TArray<>
	//Requiere Visibility
	
	/*FHitResult TraceHitResult
	PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	FVector HitLocation = TraceHitResult.ImpactPoint;
	FVector HitLocation = TraceHitResult.ImpactPoint;
	RotateTurret(HitLocation);	
	if (IsTwinSticShooter()) {
		PlayerControllerRef->bShowMouseCursor = false;
		return;
	}*/
	
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


//Deprec
void APawnTank::GetYawTurretRotate(float DeltaTime)
{
	//TurretCurrentYaw = GetTurretMesh()->GetComponentRotation().Yaw;


}



// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &APawnTank::CalculateRotateInput);
	
	//Mirar gomvo`para configurar en opciones distintas entradas.
	/*if (bIsTwinSticShooter)
	{
		UE_LOG(LogTemp, Error, TEXT("Two stick now"));
		PlayerInputComponent->BindAxis("TurnRight", this, &APawnTank::CalculateRotateInput);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("One stick now"));
		PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	}*/
	
	
	
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
	
	RotRotationDirection = Rotation;

	QuatRotationDirection = FQuat(Rotation); //Like course With cuaternions

}

void APawnTank::Move()
{
	FHitResult Hit;
	
	//GetCapsule()->AddLocalOffset(MoveDirection, true, &Hit);
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
	//GetCapsule()->AddLocalRotation(RotationDirection, true);
	AddActorLocalRotation(RotRotationDirection, true);	
	//AddActorLocalRotation(QuatRotationDirection, true); //Like course
	
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
		IsCoolDown();  //Evento blueprint
		return;
	}


	Temperatura++;

	if (Temperatura >= TempMax)bIsCooldown = true;
	else bIsCooldown = false;

	UE_LOG(LogTemp, Error, TEXT("Temperatura: %f"), Temperatura);

	IsJustFiring();



	if (GetMissingInAction() >= GetMaxPassenger() || bisTygerTank)
	{
		if (!ProjectileClassLv3)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... ProjectileClassLv3 is NONE"));
			return;
		}

		

		IsJustFiringLv3(); 

		FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotator = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* ProjectileLv3 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClassLv3, ProjectileSpawnLocation, ProjectileSpawnRotator);
		UE_LOG(LogTemp, Warning, TEXT("Fire!!... ProjectileClassLv3 "));
		ProjectileLv3->SetOwner(this);

		Temperatura+=2.f;
		FString TempMessage = FString::Printf(TEXT(" Projectile Level 3: You Temperature now is %f "), Temperatura);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempMessage);

		return;
	}

	if ( GetMissingInAction() >= GetMaxPassenger() / 2 || bisByshopTank)
	{

		if (!ProjectileClassLv2)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... ProjectileClassLv2 is NONE"));
			return;
		}
		
		IsJustFiringLv2();

		FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotator = ProjectileSpawnPoint->GetComponentRotation();
			

		AProjectileBase* ProjectileLv2 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClassLv2, ProjectileSpawnLocation, ProjectileSpawnRotator);
		UE_LOG(LogTemp, Warning, TEXT("Fire!!... ProjectileClassLv2 "));
		ProjectileLv2->SetOwner(this);
			
		Temperatura++;

		FString TempMessage = FString::Printf(TEXT(" Projectile Level 2: You Temperature now is %f "), Temperatura);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempMessage);

		return;
		
	}

	
	Super::Fire();
	
	FString TempMessage = FString::Printf(TEXT(" Projectile Level 1: You Temperature now is %f "), Temperatura);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempMessage);
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


}


void APawnTank::UpdateHeroSetting(float TMAX, float move, float rotate)
{
	TempMax = TMAX;
	MoveSpeed = move;
	RotateSpeed = rotate;
}

void APawnTank::SetTyger(bool newValue)
{
	bisTygerTank = newValue;
}

void APawnTank::SetByshop(bool newValue)
{
	bisByshopTank = newValue;
}

void APawnTank::SetPanther(bool newValue)
{
	bisPantherTank = newValue;
}









