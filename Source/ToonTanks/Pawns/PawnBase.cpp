// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanks/Pawns/PawnBase.h"

//Project
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"


//Engine and 
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"

//Components

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
//#include "Components/PrimitiveComponent.h"
//#include "Components/SceneComponent.h"

#include  "ToonTanks/Components/HealthComponent.h"



//Libraries
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShake.h"




#define OUT

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = DefaultSceneRoot;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	//RootComponent = CapsuleComp;
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	//BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetupAttachment(CapsuleComp);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent= CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}



// -- PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement Input.

void APawnBase::PawnDestroyed()
{
	HandleDestruction();
}


// Called to bind functionality to input
//void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}



void APawnBase::RotateTurret(FVector LookAtTarget)
{
	//update TurretMesh to face the LookAtTarget passed in from the child Class
	// TurretMesh->SetWorldRotation()...
	// Find Rotation value to look at. Rot Start  pos x e y del target y z de la torreta
	
	FVector StartLocation = TurretMesh->GetComponentLocation();



	
	
	

	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(
		StartLocation,		
		FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z)
	
	);
	    
	
	// Rotate Turret.

	TurretMesh->SetWorldRotation(TurretRotation);
	

	//

}



void APawnBase::MoveTank(FVector NewLocation)
{
	FVector StartLocation = BaseMesh->GetComponentLocation();

	

	BaseMesh->AddWorldOffset(NewLocation);

	if ((StartLocation.Size() - NewLocation.Size()) > 100.f)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GM MoveTank Distancia cercana"));
	}

	

}



void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class
	//at Location towards Rotation. 

	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not Fire!!... Projectile Base is NONE"));

		UE_LOG(LogTemp, Error, TEXT("in Pawn: %s "), *GetName());
		return;
	}

	{

		FVector  ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnRotator  = ProjectileSpawnPoint->GetComponentRotation();


		//UE_LOG(LogTemp, Warning, TEXT("FIRE!!"));
		//AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotator);
		//TempProjectile->SetOwner(this);
		
		AProjectileBase* ProjectileLv1 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotator);
		
		ProjectileLv1->SetOwner(this);

		
	}
	

}

void APawnBase::HandleDestruction()
{
	// Universal functionality --- 
	// Play death effects particle, sound and camera shake. 	
	
	/*float delayTime = 3.0;
	Particle =*/	
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);
		
	
	/*FTimerHandle   handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		Particle->DestroyComponent();
	   }, 
		delayTime, 
		1);*/

	// ... Then do unique child overrides:

	// -- PawnTurret - Inform GameMode Turret died -> Then Destroy() self. 

	// -- PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement Input.

}



void APawnBase::HealingMe(int Delta)
{

	UHealthComponent* HealthComp =  FindComponentByClass<UHealthComponent>();	
	
	HealthComp->Heal(Delta);

}


