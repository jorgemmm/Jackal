// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

/**

Esta es la clase base de todos Los pawns
Comportasmientos y caracter�sticas comunes a Enemigos, Players y PNJs


 */

class UCapsuleComponent;
class UStaticMeshComponent;
class USceneComponent;
class AProjectileBase;
class UHealthComponent;
class UParticleSystem;
class UParticleSystemComponent;
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

	//FORCEINLINE USceneComponent* GetDefaultRoot() const { return DefaultSceneRoot; }

	FORCEINLINE UCapsuleComponent* GetCapsule() const { return CapsuleComp; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetBaseMesh() const { return BaseMesh; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetTurretMesh() const { return TurretMesh; }

	
	/** Return the mesh for the BaseMesh */
	FORCEINLINE UHealthComponent* GetForceInlineHealthComp() const { return HealthComponent; }
	
private:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;*/


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components", 
		meta= ( AllowPrivateAccess ="true") )
		UCapsuleComponent*  CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	
	


	UParticleSystemComponent* Particle;


	

protected:
	
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectileBase> ProjectileClassLv1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		AProjectileBase* Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;


	// Death EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* DeathParticle;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake>  DeathShake;

	


	virtual void RotateTurret(FVector LookAtTarget);

	virtual void MoveTank(FVector NewLocation);

	//void Fire();
	virtual void Fire();

	//Change in child class: if turret or tank
	virtual void HandleDestruction();


	
	


public:

	UFUNCTION(BlueprintCallable)
	void HealingMe(int Delta=10);	

	


	UFUNCTION(BlueprintPure)
		UHealthComponent* GetPureHealthComponent() const;


	UFUNCTION(BlueprintCallable)
	virtual	void StartFire(FVector LookAtTarget);

	


	UFUNCTION(BlueprintImplementableEvent)
	void Save();
	
	UFUNCTION(BlueprintImplementableEvent)
	void Load();

	

};
