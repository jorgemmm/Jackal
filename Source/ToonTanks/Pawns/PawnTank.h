// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h" 


#include "PawnTank.generated.h"

/**
 
Esta es la clase base de los players
Sino heredan de  esta clase no funcionará bien el Game Mode

 
 */


class USpringArmComponent;
class UCameraComponent;



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

	

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",	meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))	
	float RotateSpeed = 100.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissingInAction", meta = (AllowPrivateAccess = "true"))
		int32 MissingInActions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissingInAction", meta = (AllowPrivateAccess = "true"))
		int32 MaxPassenger=10;

	FVector MoveDirection;
	FQuat RotationDirection;

	APlayerController* PlayerControllerRef;
	FHitResult TraceHitResult;

	
	bool bAlive = true;

	/**Indica si estamos en zona de recate*/
	bool bIsInZoneRescue = false;

	void CalculateMoveInput(float Value);
	void CalculateRotateInput(float Value);

	void Move();
	void Rotate();

	//Upgrade
	void Turbo();
	bool IsTurboEnable;	
	void EnableTurbo();

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectileBase> ProjectileClassLv2;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectileBase> ProjectileClassLv3;


	virtual void Fire() override;

protected:

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

	UFUNCTION(BlueprintCallable)
		void SetIsInZoneRescue();

	UFUNCTION(BlueprintCallable)
		void ResetIsInZoneRescue();

};
