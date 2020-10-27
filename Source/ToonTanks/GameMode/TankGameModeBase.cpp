// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

//Engine
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

//Components
#include "Toontanks/GameInstance/TankGameGameInstance.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/Pawns/PawnMissingCombat.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"



//Utils
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#define OUT

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// Get references and game win/lose conditions.
	//TargetTurrets = GetTargetTurretCount();
	//PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Call HandleGameStart to initialise the start countdown, turret activation, pawn check etc.
	HandleGameStart();

	LevelNames =			  {TEXT("Level1"), TEXT("Level2"), TEXT("Level3"), TEXT("Level4"),TEXT("Level5") };

	//MissingInActionsByLevel = { 8, 8, 16, 16,32 };  //number * 3 number of enemies  


}


void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	// Check what type of Actor died.  
	UE_LOG(LogTemp, Warning, TEXT("A Pawn Died"));
	//If Player -> go to lose condition.
	if (DeadActor == PlayerTank)
	{
		

		UE_LOG(LogTemp, Warning, TEXT("A PlayerPawn Died"));
		PlayerTank->PawnDestroyed();
		HandleGameOver(false);

		/*if (PlayerControllerRef)
		{
			PlayerControllerRef->SetPlayerEnabledState(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GM.ActorDied PlayerController not Found or none"));
		}*/
	
	
	}
	  //If Turret, tally.
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("A EnemyPawn Died"));
		
		DestroyedTurret->PawnDestroyed();
		Score+=5;

		//Score+=DestroyedTurret->ScoreValue Upgrade
		//Score+=DestroyedTurret->GetScoreValue() Upgrade

		TargetTurrets--;

		/*if (TargetTurrets == 0)
		{
			HandleGameOver(true);
		}*/
	}
	else if (APawnMissingCombat* MissigInAction = Cast<APawnMissingCombat>(DeadActor))
	{
		   Score += 5;
		  //LLámalo desde la zona de rescate
		     MissinInActions++;
			if (MissinInActions >= MissingRequired  && TargetTurrets == 0)
			{
				//TargetTurrets = 0;
				MissinInActions = 0;
				//MissingRequired = 0;
				HandleGameOver(true);
			}
			else
			{

				UE_LOG(LogTemp, Warning, TEXT("Enemies Left: %f"), TargetTurrets);
				UE_LOG(LogTemp, Warning, TEXT("Misssing Left: %i"), (MissingRequired- MissinInActions));

			}

	}

}


int32 ATankGameModeBase::GetTargetTurretCount() const
{
	// Get references and game win/lose conditions.
	TSubclassOf<APawnTurret> ClassToFind;
	ClassToFind = APawnTurret::StaticClass();
	
	TArray<AActor*> TurretActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT TurretActors);
	
	return TurretActors.Num();

}


int32 ATankGameModeBase::GetMissingRequired() const
{

	//Llma aesta fun ción desde bluepront cada vez que destruyas una 
	//carcel de drones

	TSubclassOf<APawnMissingCombat> ClassToFind;
	ClassToFind = APawnMissingCombat::StaticClass();

	TArray<AActor*> MissingActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT MissingActors);

	return MissingActors.Num();



}

void ATankGameModeBase::SetMissingRequired()
{
	MissingRequired = GetMissingRequired();
}


void ATankGameModeBase::HandleGameStart()
{
	//in beginplay¿?
	// Get references and game win/lose conditions.
	TargetTurrets = GetTargetTurretCount();

	MissingRequired = GetMissingRequired();


	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	//En online, 0 es el controller local
	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	

	TankGI = Cast<UTankGameGameInstance>( GetGameInstance() );



	/**Begin Check this */
	
	
	/**End Check this */

	// initialise the start countdown, turret activation, pawn check etc.
	// Call Blueprint version GameStart();
	
	GameStart();

	if (PlayerControllerRef!=nullptr)
	{
		//DelayToStart();
		PlayerControllerRef->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableHandle;

		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState,true);		
		GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
		
		

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameStart PlayerController not Found or none"));
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	// See if the player has destroyed all the turrets, show win result.   
    // else if turret destroyed player, show lose result.    
    // Call blueprint version GameOver();
	 //in older version

	//in this version the player goes to other map

	GameOver(PlayerWon);

	if (!PlayerWon)
	{
		DelayToStart();
		return;
	}
	

	

	if (PlayerControllerRef)
	{
		int32 LevelID=0;
		
		if (!TankGI)
		{
			UE_LOG(LogTemp, Error, TEXT("GM.HandleGameOver PyerWon not Found or none"));
			DelayToStart();
			return;
		}

		TankGI->UpdateLevelID();
		

		LevelID = TankGI->GetLevelID();	
		
		//Para recibir feedback en itch.io or discord
		int32 lastMissionMap = 1; //fOR sHIPPING lastMission=5+         
		if(LevelID>= lastMissionMap)GameOver(true);
		

		UGameplayStatics::OpenLevel(GetWorld(), LevelNames[LevelID]);
		
	
		
		
		

		

		//PawnTank->GetHealthcomponent->Health = PawnTank->GetHealthComponent->DefaultHealth;
		//		
		//GameStart();	
		//
		//Después de Gamestart
		//Disable Pawn HandleDestruction (PawnTank->  Visibilitity, Tick , Setbisalive)
	    //   
		//PlayerTank->SetPlayerReAlive;
	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameOver PlayerController not Found or none"));
	}

}


int32 ATankGameModeBase::GetScore() const
{
	return Score;
}


void ATankGameModeBase::SetScore(int32 Delta)
{
	Score+=Delta;
}


//Deprecated
void ATankGameModeBase::DelayToStart() 
{
	
	if(PlayerControllerRef)
	PlayerControllerRef->SetPlayerEnabledState(false);
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.DelayToStartw PlayerController not Found or none"));
	}
	


	//Vamos a utilizar un timer delegado
	//porque dispara una función en otro objeto
	FTimerHandle PlayerEnableHandle;

	//FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState,true);		
	//GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);

	
	GetWorldTimerManager().ClearTimer(PlayerEnableHandle);
	GetWorldTimerManager().SetTimer(
		PlayerEnableHandle,
		this,
		&ATankGameModeBase::EnableController,
		StartDelay,
		false,
		3.f
	);
}

//Deprecated
void ATankGameModeBase::EnableController()
{

	
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetPlayerEnabledState(true);
		PlayerControllerRef->RestartLevel();

		

	}
	else
	{

		UE_LOG(LogTemp, Error, TEXT("GM.EnableContoller PlayerController not Found or none"));
	}


}