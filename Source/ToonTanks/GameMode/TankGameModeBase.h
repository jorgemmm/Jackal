// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

/**
 * 
 */

class APawnTurret;
class APawnTank;
class APlayerControllerBase;
class UCameraShake;
class UTankGameGameInstance;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;



private:

	int32 TargetTurrets = 0;
	APlayerControllerBase* PlayerControllerRef;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	APawnTank* PlayerTank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		int StartDelay = 3;


	int32 GetTargetTurretCount();
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	TArray<FName> LevelNames;
	
	
	void DelayToStart();//Deprecated
	void EnableController();//Deprecated

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int32 Score =0;

	UTankGameGameInstance* TankGI;

public:
	void ActorDied(AActor* DeadActor);
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

	
	UFUNCTION(BlueprintPure)
	int32 GetScore() const;

	
	UFUNCTION(BlueprintCallable)
	void SetScore(int32 Delta);
	
};
