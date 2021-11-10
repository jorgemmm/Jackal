// Víctor J. Pasín Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PawnTurret.h"  
#include "PawnTankEnemy.generated.h"





/**
 * 
 */

UCLASS()
class TOONTANKS_API APawnTankEnemy: public APawnTurret //public APawnBase
{
	GENERATED_BODY()
	
public:
	APawnTankEnemy();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CheckDistance(APawn* PawnTank);

	//Change in child class: if enemy tank
	virtual void HandleDestruction() override;

	//Change override in child class: if enemy tank
	virtual float ReturnDistanceToPlayer() override;

private:

	

	FVector Velocity, Translation;

	void Move();
	FRotator RotateBase(FVector LookAtTarget);
	
	
	/**Distance To stop and ready to shot: when target is at FireRange*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float DistToShoot = 1000.0f;

protected:
	

};
