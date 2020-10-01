// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "Engine/EngineTypes.h"
//#include "GameFramework/Pawn.h"

#include "PawnTurret.generated.h"

/**
 * 
 */

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret: public APawnBase
{
	GENERATED_BODY()
	

public:
	APawnTurret();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

	
	float GetScoreValue();


	APawnTank* PlayerPawn;

	//Change override in child class: if enemy tank
	virtual float ReturnDistanceToPlayer();

private:
	
	/**Disparo cada FireRate segundos*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float ScoreValue = 1;	


	

	
	
	

protected:

	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();

	//float ReturnDistanceToPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float FireRange = 500.0f;
	
};
