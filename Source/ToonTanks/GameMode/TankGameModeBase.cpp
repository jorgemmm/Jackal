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
	 
	
	FString CurrentLevelName =  UGameplayStatics::GetCurrentLevelName( GetWorld() , true) ;

	LevelNames.Emplace(TEXT("Level1"));
	LevelNames.Emplace(TEXT("Level2"));
	LevelNames.Emplace(TEXT("Level3"));
	LevelNames.Emplace(TEXT("Level4"));
	
	

	//int32 Index;
	//if (StrArr.Find(TEXT("Hello"), Index))
	//{
	//	// Index == 3
	//}	
	if ( !LevelNames.Find(CurrentLevelName, LevelID) )  LevelID = 0;

	
    
	//Estos son los drones que pones en el mapa
	//2

	MissingInActionsByLevel.Init(6, 5);

	for (size_t i = 0; i < MissingInActionsByLevel.Num(); i++)
	{
		if (i > 0) MissingInActionsByLevel[i] += 1;
	}

	
	HandleGameStart();

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
	else 
	{


		if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("A EnemyPawn Died"));
			
			//DestroyedTurret->HandleDestruction();
			DestroyedTurret->PawnDestroyed();

			Score += 5;
			TargetTurrets--;
			
			UE_LOG(LogTemp, Warning, TEXT("A EnemyPawn Died, Left: %f"), TargetTurrets);

			
			//HandleMisionRequired(); Deprec delete

		}
		
		if (APawnMissingCombat* MissigInAction = Cast<APawnMissingCombat>(DeadActor))
		{
			
			//MissigInAction->HandleDestruction();
			MissigInAction->PawnDestroyed();
			//Call from rescue zone
			Score += 5;
			MissinInActions++;
			//MissingInActionsByLevel[LevelID];			
			//MissingRequired--;
			UE_LOG(LogTemp, Warning, TEXT("A Drone rescued, TotalRescue: %d, Drone rescued: %d"), MissinInActions, MissingRequired);
			
			
			//HandleMisionRequired(); Deprec delete
		}

		

		
	}
	

	//HandleMisionRequired();

}


void ATankGameModeBase::HandleMision()
{
	const int32 EnemyDestroyed = TargetTurrets;
	const int32 Dronesrescue = MissinInActions;//MissingInActionsByLevel[LevelID];
	UE_LOG(LogTemp, Error, TEXT("A EnemyPawn Died, Left: %d"), EnemyDestroyed);
	UE_LOG(LogTemp, Error, TEXT("A Dr0ne rescued: %d, Missings : %d"),Dronesrescue, MissingRequired);
	if (EnemyDestroyed == 0 && Dronesrescue >= MissingRequired)
	{
		UE_LOG(LogTemp, Error, TEXT("Mission acomplished"));
		HandleGameOver(true);

	}
}


int32 ATankGameModeBase::GetMissingCount() const
{

	return MissinInActions;

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

	
	return MissingInActionsByLevel[LevelID];

}

int32 ATankGameModeBase::GetMissingInMap() const
{
	//Llma aesta fun ci�n desde bluepront cada vez que destruyas una 
	//carcel de drones

	TSubclassOf<APawnMissingCombat> ClassToFind;
	ClassToFind = APawnMissingCombat::StaticClass();

	TArray<AActor*> MissingActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT MissingActors);


	return MissingActors.Num();
}

void ATankGameModeBase::SetMissingRequired()
{
	//MissingRequired = GetMissingRequired();
	const int32 CurrentDrones =  FMath::Clamp(GetMissingInMap() , 1 , GetMissingInMap());
		
	

	MissingInActionsByLevel[LevelID] += CurrentDrones;
	
	
}




void ATankGameModeBase::HandleGameStart()
{
	//in beginplay�?
	// Get references and game win/lose conditions.
	LevelID = 0;
	TargetTurrets = GetTargetTurretCount();
	MissinInActions = 0;
	MissingRequired = GetMissingRequired();
	//SetMissingRequired();
	//MissingRequired = GetMissingRequired();

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	//En online, 0 es el controller local
	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	

	TankGI = Cast<UTankGameGameInstance>( GetGameInstance() );


	//PlayerTank->Load(); // Se puede usar para salvar datos entre partidas

	if (!TankGI)
	{
		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameStart GetGameInstance not Found or none"));
		return;

	}

	Score = TankGI->GetGI_Score(); // de momento sólo datos entre niveles dentro de la misma partida


	/**Begin Check this */
	
	
	/**End Check this */

	// initialise the start countdown, turret activation, pawn check etc.
	// Call Blueprint version GameStart();
	
	GameStart();

	if (!PlayerControllerRef)
	{

		UE_LOG(LogTemp, Error, TEXT("GM.HandleGameStart PlayerController not Found or none"));
		

	}

	//DelayToStart();
	PlayerControllerRef->SetPlayerEnabledState(false);

	FTimerHandle PlayerEnableHandle;

	FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
	GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);

	GetWorldTimerManager().SetTimer(TH_TimeMissionHandle, this, &ATankGameModeBase::HandleMision,3.f ,true, 2.f);
	
}


//Deprec delete
void ATankGameModeBase::HandleMisionRequired()
{
	if ( (MissinInActions >= MissingRequired)  && TargetTurrets <= 0)
	//if(MissingRequired==0 && TargetTurrets == 0)
	{
		//TargetTurrets = 0;
		//MissinInActions = 0;
		//MissingRequired = 0;
		HandleGameOver(true);
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("Enemies Left: %f"), TargetTurrets);
		UE_LOG(LogTemp, Warning, TEXT("Misssing Required Left: %i"), (MissingRequired ));
		UE_LOG(LogTemp, Warning, TEXT("Misssing Rescued  Left: %i"), (MissinInActions));


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
		//LevelID=0;
		
		if (!TankGI)
		{
			UE_LOG(LogTemp, Error, TEXT("GM.HandleGameOver PlayerWon not Found or none"));
			DelayToStart();
			return;
		}

		PlayerTank->Save();

		TankGI->UpdateScore(Score);
		
		//TankGI->UpdateLevelID();		
		TankGI->UpdateLevelID(LevelID+1);
		

		LevelID = TankGI->GetLevelID();	
		
		//Para recibir feedback en itch.io or discord
		//fOR sHIPPING lastMission=5+
		int32 lastMissionMap = 4;    


		if( LevelID >= lastMissionMap) GameOver(true);
		

		FName TargetLevel = FName(*LevelNames[LevelID]);
		
		UGameplayStatics::OpenLevel(GetWorld(), TargetLevel);
		
	
		
		
		

		

		//PawnTank->GetHealthcomponent->Health = PawnTank->GetHealthComponent->DefaultHealth;
		//		
		//GameStart();	
		//
		//Despu�s de Gamestart
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
	
	if (PlayerControllerRef) {
		//PlayerControllerRef->SetPlayerEnabledState(false);
		PlayerControllerRef->UnPossess();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GM.DelayToStartw PlayerController not Found or none"));
	}
	


	//Vamos a utilizar un timer delegado
	//porque dispara una funci�n en otro objeto
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
		//PlayerControllerRef->SetPlayerEnabledState(true);
		
		PlayerControllerRef->Possess(PlayerTank);
		
		PlayerControllerRef->RestartLevel();

		

	}
	else
	{

		UE_LOG(LogTemp, Error, TEXT("GM.EnableContoller PlayerController not Found or none"));
	}


}