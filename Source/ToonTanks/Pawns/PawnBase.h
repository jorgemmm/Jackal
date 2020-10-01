// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"


class UCapsuleComponent;
class UStaticMeshComponent;
class USceneComponent;
class AProjectileBase;
class UHealthComponent;
class UParticleSystem;
class USoundBase;
class UCameraShake;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PawnDestroyed();   


	FORCEINLINE class UCapsuleComponent* GetCapsule() const { return CapsuleComp; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE class UStaticMeshComponent* GetBaseMesh() const { return BaseMesh; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE class UStaticMeshComponent* GetTurretMesh() const { return TurretMesh; }


private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components", 
		meta= ( AllowPrivateAccess ="true") )
	UCapsuleComponent*  CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	AProjectileBase* Projectile;

protected:

	// Death EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* DeathParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake>  DeathShake;


	void RotateTurret(FVector LookAtTarget);

	void MoveTank(FVector NewLocation);

	void Fire();

	//Change in child class: if turret or tank
	virtual void HandleDestruction();


	void ResetFury();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		float FuryDuration=3.f;


public:

	UFUNCTION(BlueprintCallable)
	void HealingMe();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	int32 Fury = 1;

	UFUNCTION(BlueprintCallable)
	void FuryMe();


	UFUNCTION(BlueprintPure)
	float GetFury();

	



};
