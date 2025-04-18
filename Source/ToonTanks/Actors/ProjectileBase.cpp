﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
//#include "ToonTanks/Actors/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Camera/CameraShake.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

//"debug code" for console command
//int32 DebugWeaponDrawing = 0;
//FAutoConsoleVariableRef   CVARDebugWeaponDrawing(
//	TEXT("ToonTanks.DebugProjectile"),
//	DebugWeaponDrawing,
//	TEXT("Draw Dbug Sphere for Projectile explosion"),
//	ECVF_Cheat);

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;
	
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.0f;

	PointDamage = BaseDamage;

	RadialDamage = BaseDamage / 4;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();	
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Try to get a reference to the owning class. 
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		UE_LOG(Logtemp, Error, TEXT("Owner not found or is nulptr"));
		return;
	}


	PointDamage =  BaseDamage;
	RadialDamage = BaseDamage / 4;
	
	TArray<AActor*> IgnorethisActors;
	IgnorethisActors.AddUnique(this);
	IgnorethisActors.AddUnique(MyOwner);

	//Apply damage to other actor
	if (OtherActor != NULL && OtherActor != this && OtherActor != MyOwner)
	{
    
		UE_LOG(LogTemp, Warning, TEXT("FIRE!! PointDamage: %f"), PointDamage);
		

		UGameplayStatics::ApplyDamage(
			OtherActor,
			PointDamage,
			MyOwner->GetInstigatorController(),
			this,
			DamageType
		);


		UE_LOG(LogTemp, Warning, TEXT("FIRE!! RadialDamage: %f to %s"), RadialDamage, *OtherActor->GetName());
		
		UGameplayStatics::ApplyRadialDamage(
			OtherActor,
			RadialDamage,
			Hit.ImpactPoint,//GetActorLocation(),
			RadialRange,
			DamageType,
			IgnorethisActors,
			this,

			MyOwner->GetInstigatorController()

		);

		//Uncoment this if you have uncoment the "debug code" for console command
		//if (DebugWeaponDrawing > 0)
		{

			//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, RadialRange, 8.f, FColor::Yellow, false, 3.0f, 0, 1.0f);
		}

		//You can add force to projectil 
		//Projectil must  have collision reaady to hit to target
		//and target must have physhic enable. 
		//ProjectileMesh->AddForce(this->GetVelocity() * 100);

		// Particle System
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			HitParticle,
			GetActorLocation()
		);

		// Audio FX
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			HitSound,
			GetActorLocation()
		);

		//Camera Shake
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitShake);

	}


	// Do a bunch of effects here during polish phase.
	
	
	
	//Destroy proyectile after Hit
	Destroy(); 
}

void AProjectileBase::AddDamage(float Multipier)
{
	PointDamage = BaseDamage* Multipier* Multipier;	
	RadialDamage =  (BaseDamage / 4)* Multipier;
}


float AProjectileBase::GetPointDamage() const
{
	return PointDamage;
}


float AProjectileBase::GetRadialDamage() const
{
	return RadialDamage;
}



