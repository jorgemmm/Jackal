// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


using namespace std;
#include <iostream>
#include <ctime>

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

	int32 DroneLevel1;
	int32 DroneLevel2;
	
	TArray<int32> DroneLevel;// [] 

	int32 TargetTurrets = 0;
	
	int32 SpawnPointId=0;
	
	/*Missing in Actions in Map; you have to search it*/
	int32 MissingRequired=0;

	/*Missing in actions rescued; when equal MissingRquired Mission is Success*/
	int32 MissinInActions = 0;

	APlayerControllerBase* PlayerControllerRef;


	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	APawnTank* PlayerTank;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawnTank> HeroTankClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		int StartDelay = 3;


	

	void HandleGameStart();
	void HandleMisionRequired(); //Depecated delete
	void HandleGameOver(bool PlayerWon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		TArray<FString> LevelNames; 
	/* Num máx de drons in Level Min=Max/10**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		int32 NumMaxDronsInMap = 36;

	void DelayToStart();
	void EnableController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int32 Score =0;

	/**To choose a level at the begining*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		int32 LevelID = 0;

	UTankGameGameInstance* TankGI;

	FTimerHandle TH_TimeMissionHandle;

public:
	void ActorDied(AActor* DeadActor);
	void HandleMision();
	

	
	
	/**Llamma a esta función cada vez que llegues a una zona de evacuación*/
	UFUNCTION(BlueprintCallable)
		void AddSpawnPointId();

	UFUNCTION(BlueprintPure)
	int32 GetSpawnPointId() const;


	UFUNCTION(BlueprintCallable)
		void SetMissingRequired();

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

	
	UFUNCTION(BlueprintPure)
	int32 GetTargetTurretCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetMissingInMap() const;

	UFUNCTION(BlueprintPure)
		int32 GetMaxMissingInMap() const;

	UFUNCTION(BlueprintPure)
	int32 GetScore() const;

	UFUNCTION(BlueprintPure)
	int32 GetMissingCount() const;

	UFUNCTION(BlueprintPure)
	int32 GetMissingRequired() const;


	UFUNCTION(BlueprintCallable)
	void SetScore(int32 Delta);



/*Deprected delete or evaluation*/
	/** No en uso para esta lógica*/
	UFUNCTION(BlueprintCallable)
	void ReSpawn();
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
		TArray<int32> MissingInAcRequiredByLevel;
		*/

	
};
