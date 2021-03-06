// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h" 


#include "PawnTank.generated.h"

/**
 
Esta es la clase base de los players
Sino heredan de  esta clase no funcionar� bien el Game Mode

 
 */


class USpringArmComponent;
class UCameraComponent;

class UParticleSystem;
class USoundBase;
class APlayerController;

UCLASS()
class TOONTANKS_API APawnTank: public APawnBase
{
	GENERATED_BODY()

public:
	
	APawnTank();
	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	bool GetPlayerAlive();	
	void SetPlayerReAlive();

	virtual void HandleDestruction() override;

	//virtual void RotateTurret(FVector LookAtTarget) override;

	UFUNCTION(BlueprintImplementableEvent)
	void IsCoolDown();

	UFUNCTION(BlueprintImplementableEvent)
	void IsJustFiring();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",	meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* TurretSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TurretCamera;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HotParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	USoundBase* HotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))	
	float RotateSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))
	float TurretRotateSpeed = 75.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissingInAction", meta = (AllowPrivateAccess = "true"))
		int32 MissingInActions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissingInAction", meta = (AllowPrivateAccess = "true"))
		int32 MaxPassenger=10;

	FVector MoveDirection, LastLocation;
	FQuat RotationDirection;

	APlayerController* PlayerControllerRef;
	//FHitResult TraceHitResult;

	
	bool bAlive = true;

	/**Indica si estamos en zona de recate*/
	bool bIsInZoneRescue = false;

	void CalculateMoveInput(float Value);
	void CalculateRotateInput(float Value);

	void Move();
	void Rotate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))
	float Temperatura;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))
	float TempMax=75;
	void CoolDown();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Settings", meta = (AllowPrivateAccess = "true"))
	bool bIsCooldown;
	//Upgrade
	

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectileBase> ProjectileClassLv2;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectileBase> ProjectileClassLv3;

	//Deprecated a nivel de game mode y en blueprint con nodo DoOnce
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
		USoundBase* HealthySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
		USoundBase* LowHealthSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
		USoundBase* DeathlyHealthSound;*/

	

	

	virtual void Fire() override;

	float CurrentYaw;

protected:
	void Rotate(float DeltaTime);
	
	

	


public:

	UFUNCTION(BlueprintPure)
		int32 GetMissingInAction() const;
	
	
	

	UFUNCTION(BlueprintCallable)
		void SetMissingInAction(int32 NewRescued);

	

	
	UFUNCTION(BlueprintPure)
		int32 GetMaxPassenger() const;

	

	/**Indica si estamos en zona de recate*/
	UFUNCTION(BlueprintPure)
		bool GetIsInZoneRescue() const;

	UFUNCTION(BlueprintPure)
		float GetTemperatura() const;

	/*UFUNCTION(BlueprintCallable)
		void ResetTemperature();*/



	UFUNCTION(BlueprintCallable)
		void SetIsInZoneRescue();

	UFUNCTION(BlueprintCallable)
		void ResetIsInZoneRescue();

	UFUNCTION(BlueprintCallable)
	void ChangeCameraView(bool bInBaseRoot);

};
