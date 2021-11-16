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

#include "ToonTanks/Actors/TargetSpawn.h"

#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"





//Utils
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#define OUT

const int32 LASTMISSION = 3;//3 in Production;

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	//Get references and game win/lose conditions.
	//TargetTurrets = GetTargetTurretCount();
	//PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	
	 
	
	//mejor por datables
	FString CurrentLevelName =  UGameplayStatics::GetCurrentLevelName( GetWorld() , true) ;

	LevelNames.Emplace(TEXT("Level3"));
	LevelNames.Emplace(TEXT("Level4"));
	LevelNames.Emplace(TEXT("Level5"));
	
	//Debug
	LevelNames.Emplace(TEXT("SandBox"));
	

	
	if ( !LevelNames.Find(CurrentLevelName, LevelID) )  LevelID = 0; //0 in production


	//Call HandleGameStart to initialise the start countdown, turret activation, pawn check etc.
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
			//MissingInAcRequiredByLevel[LevelID];			
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
	const int32 Dronesrescue = MissinInActions;//MissingInAcRequiredByLevel[LevelID];
	UE_LOG(LogTemp, Error, TEXT("A EnemyPawn Died, Left: %d"), EnemyDestroyed);
	UE_LOG(LogTemp, Error, TEXT("A Dr0ne rescued: %d, Missings : %d"),Dronesrescue, MissingRequired);
	if (EnemyDestroyed == 0 && Dronesrescue >= MissingRequired)
	{
		UE_LOG(LogTemp, Error, TEXT("Mission acomplished"));
		HandleGameOver(true);

	}
}


/**Llamma a esta función cada vez que llegues a una zona de evacuación*/
void ATankGameModeBase::AddSpawnPointId()
{	
	//No más de 3 puntos de Spawning.
	SpawnPointId=FMath::Clamp(++SpawnPointId,0,3);
}


int32 ATankGameModeBase::GetSpawnPointId() const
{
	return SpawnPointId;
}

void ATankGameModeBase::SetMissingRequired()
{
	//MissingRequired = GetMissingRequired();
	const int32 CurrentDrones = FMath::Clamp(GetMissingInMap(), 0, GetMissingInMap());

	MissingRequired += CurrentDrones;


}


int32 ATankGameModeBase::GetMissingCount() const
{

	//To update in Widget BP the drones rescued
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

	//Get references and game win/lose conditions.	

	//return MissingInAcRequiredByLevel[LevelID];
	return MissingRequired;
}


 /* Llama aesta función desde bluepront cada vez que destruyas una 
    carcel de drones
	Llámala tb desde el HandleGameStart() para capturar a los drones placed in Map
	place some drones in map to avoid nulptr reference*/


int32 ATankGameModeBase::GetMissingInMap() const
{
	
	
	TSubclassOf<APawnMissingCombat> ClassToFind;
	ClassToFind = APawnMissingCombat::StaticClass();

	TArray<AActor*> MissingActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, OUT MissingActors);


	return MissingActors.Num();
}

int32 ATankGameModeBase::GetMaxMissingInMap() const
{
	return NumMaxDronsInMap;
}





void ATankGameModeBase::HandleGameStart()
{
	//in beginplay�?
	// Get references and game win/lose conditions.
	LevelID = 0;
	TargetTurrets = GetTargetTurretCount();
	MissinInActions = 0;
	//place some drones in map to avoid nulptr reference
	MissingRequired = GetMissingInMap();

	
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


	
	// initialise the start countdown, turret activation, pawn check etc.
	// Call Blueprint version GameStart();
	
	GameStart();

	

	//Delay To Start();
	PlayerControllerRef->SetPlayerEnabledState(false);

	FTimerHandle PlayerEnableHandle;

	//
	FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
	GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);

	
	//Blucle de juego
	GetWorldTimerManager().SetTimer(TH_TimeMissionHandle, this, &ATankGameModeBase::HandleMision,3.f ,true, 2.f);
	
}



void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	// See if the player has destroyed all the turrets, show win result.   
    // else if turret destroyed player, show lose result.    
    // Call blueprint version GameOver();
	//in older version
	//in this version the player goes to other map

	GameOver(PlayerWon);
	
	//You Lost
	if (!PlayerWon)
	{
		DelayToStart();
		return;
	}
	

	
	//You Win
	if (PlayerControllerRef)
	{
		//LevelID=0;
		
		if (!TankGI)
		{
			UE_LOG(LogTemp, Error, TEXT("GM.HandleGameOver PlayerWon not Found or none"));
			DelayToStart();
			return;
		}

		//Evento save Mejor poner save en codigo
		PlayerTank->Save();

		TankGI->UpdateScore(Score);	
		
		
		//TankGI->UpdateLevelID();		
		TankGI->UpdateLevelID(LevelID+1);

		
		LevelID = TankGI->GetLevelID();	
		
		//Para recibir feedback en itch.io or discord
		//fOR sHIPPING lastMission=5+
		
		//int32 lastMissionMap = 4;    


		if( LevelID >= LASTMISSION) GameOver(true);
		

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


//Cuando game over Si Win/Lost
void ATankGameModeBase::DelayToStart() 
{
	
	if (PlayerControllerRef)
	{
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

		//ReSpawn();

		

	}
	else
	{

		UE_LOG(LogTemp, Error, TEXT("GM.EnableContoller PlayerController not Found or none"));
	}


}






//Deprec delete
void ATankGameModeBase::HandleMisionRequired()
{
	if ((MissinInActions >= MissingRequired) && TargetTurrets <= 0)
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
		UE_LOG(LogTemp, Warning, TEXT("Misssing Required Left: %i"), (MissingRequired));
		UE_LOG(LogTemp, Warning, TEXT("Misssing Rescued  Left: %i"), (MissinInActions));


	}
}




/*Deprecated delete or for evaluation*/

/** No en uso para esta lógica*/
void ATankGameModeBase::ReSpawn()
{
	// Get references and game win/lose conditions.
	// TSubclassOf<APawnTank> ClassToFind;
	// ClassToFind = APawnTank::StaticClass();

	UWorld* const World = GetWorld();

	if (!World)
		if (!World)
		{
			UE_LOG(LogTemp, Error, TEXT("world no cargado ?¿?¿?"));
			return;
		}

	//Si hay algún Hero debug o bug se destruyen
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, APawnTank::StaticClass(), OUT FoundActors);
	for (auto hero : FoundActors)
	{
		hero->Destroy();
	}
	/*for (size_t i = 0; i < TargetActors.Num(); i++)
	{

	}*/

	//Spawn hero at target Spawn Points "SpawnPointId" Se actualiza en las zonas de evacuación
	FoundActors.Empty();
	//TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsOfClass(World, ATargetSpawn::StaticClass(), OUT FoundActors);


	if (FoundActors.Num() > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		FVector loc = FVector::FVector(0, 0, 112);
		FTransform Transform = FTransform::FTransform(loc);
		FoundActors[SpawnPointId]->GetTransform();

		PlayerTank = World->SpawnActor<APawnTank>(HeroTankClass, Transform, SpawnParams);

		UE_LOG(LogTemp, Warning, TEXT("Spawned"));


		DelayToStart(); 
			
		
		/*
		Esta lógica no va con el resto
		PlayerControllerRef =  Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(World, 0));		

		if (!PlayerControllerRef)
		{
			UE_LOG(LogTemp, Error, TEXT("GM.HandleGameStart PlayerController not Found or none"));
			return;

		}

		PlayerControllerRef->Possess(PlayerTank);
		*/
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No TargetSpawn Actor Founded in level"));
	}



}



