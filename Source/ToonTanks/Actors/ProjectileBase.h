// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;
class UParticleSystemComponent;
class USoundBase;
class UCameraShake;

/**
PawnBase for turretenemy and PankTank 
*/

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleTrail;
	

	UPROPERTY(EditDefaultsOnly, Category = "Projec_Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed=1300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Projec_Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	
	/*Daño base Puntual o individual y 1/4 del radial */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projec_Damage", meta = (AllowPrivateAccess = "true"))
	float BaseDamage = 20.f;
	/*Radio de la explosión alcance del daño radial*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projec_Damage", meta = (AllowPrivateAccess = "true"))
	float RadialRange = 25.0f;

	
	/*Daño Puntual*/
	UPROPERTY()
	float PointDamage;

	/*Daño radial*/	
	UPROPERTY()
	float RadialDamage;	
	
	// EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projec_Effects", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projec_Effects", meta = (AllowPrivateAccess = "true"))
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projec_Effects", meta = (AllowPrivateAccess = "true"))
		USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projec_Effects", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake>  HitShake;


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:


public:

	UFUNCTION(BlueprintCallable)
		void AddDamage(float Multipier);

	UFUNCTION(BlueprintPure)
		float GetPointDamage() const;


};
