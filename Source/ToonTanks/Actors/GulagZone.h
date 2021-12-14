// Victor J. Pasin Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GulagZone.generated.h"



//class UCapsuleComponent;
class UStaticMeshComponent;
//class USceneComponent;
//class AProjectileBase;
class UHealthComponent;
//class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class UCameraShake;
class APawnMissingCombat;

UCLASS()
class TOONTANKS_API AGulagZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGulagZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void GulagZoneDestroyed();

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetWallMesh() const { return WallMesh; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetFrontMesh() const { return FrontMesh; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetCritalLeftMesh() const { return CritalLeftMesh; }

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetCritalRightMesh() const { return CritalRightMesh; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* FrontMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CritalRightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CritalLeftMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;

	/*In blueprint niagara particles*/
	//UParticleSystemComponent* Particle;

	//Change in child class: if turret or tank
	void HandleDestruction();

	void SpawnMissing();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake>  DeathShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APawnMissingCombat> MissingInActionClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		TArray<APawnMissingCombat*> MissingInActions;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		int32 DroneDispersion = 100;



public:
	/** Get CPP HealthComponent custom myself from this actor  */
	UFUNCTION(BlueprintPure)
		UHealthComponent* GetPureHealthComponent() const;

	/** Fire an Event at Blueprint to Active a niagara particle system */
	UFUNCTION(BlueprintImplementableEvent)
		void NiagaraStart();

};
