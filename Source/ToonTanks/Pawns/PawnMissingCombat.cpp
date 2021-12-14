
// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnMissingCombat.h"
#include "PawnTank.h"
//#include "ToonTanks/Pawns/PawnMissingCombat.h"
//#include "ToonTanks/Pawns/PawnTank.h"

#include "GameFramework/Pawn.h"

//components
#include  "../Components/HealthComponent.h"
#include  "../GameMode/TankGameModeBase.h"
//#include  "ToonTanks/Components/HealthComponent.h"
//#include  "ToonTanks/GameMode/TankGameModeBase.h"

#include "ToonTanks/Actors/RescueZone.h"

//Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

//#include "Blueprint/AIBlueprintHelperLibrary.h"

//Components
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


#include "GameFramework/Controller.h"


APawnMissingCombat::APawnMissingCombat()
{


	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = GetCapsule();
	//USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	////RootComponent = SphereComponent;
	//SphereComponent->InitSphereRadius(400.0f);
	//SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APawnMissingCombat::NotifyActorBeginOverlap);



	//RootComponent = GetCapsule();

	ZoneToRescue = CreateDefaultSubobject<USphereComponent>(TEXT("Zone To Rescue"));
	ZoneToRescue->SetupAttachment(GetBaseMesh());	
	
	//ZoneToRescue->SetupAttachment(RootComponent);
	ZoneToRescue->InitSphereRadius(300.0f);	
	ZoneToRescue->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	
	
	// GetCapsule()->
	//Debe coincidir la signatura de la función  Para enlazar  D.I. correctamente
	
	ZoneToRescue->OnComponentBeginOverlap.AddDynamic(this, &APawnMissingCombat::OnBOver_Rescue);
	//ZoneToRescue->OnComponentHit.AddDynamic(this, &APawnMissingCombat::OnHit_Rescue);
	

	
	
	

}

void APawnMissingCombat::BeginPlay()
{
	Super::BeginPlay();
	
	
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	FoundMissings(); //Because Timer fires at 10 seconds and must be somethig in  RescueVolumeActors[0]
	
					 //Cada 1/2 segundos, a partir de 10 segundos de empezar 
	GetWorld()->GetTimerManager().ClearTimer(TimerMissingHandler);

	
	GetWorld()->GetTimerManager().SetTimer(TimerMissingHandler, this, &APawnMissingCombat::MissingDistanceHandler, .5f, true,.5f); 
}



void APawnMissingCombat::FoundMissings()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARescueZone::StaticClass(), FoundActors);
	

	for (auto Actor : FoundActors)
	{
		ARescueZone* RescueVolumeActor = Cast<ARescueZone>(Actor);
		if (RescueVolumeActor)
		{
			RescueVolumeActors.AddUnique(RescueVolumeActor);
			//check(GEngine != nullptr);

			// Display a debug message for five seconds. 
			// The -1 "Key" value argument prevents the message from being updated or refreshed.
			/*FString TempMessage = FString::Printf(TEXT("There are new Rescue zone: %s"), *RescueVolumeActor->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempMessage);*/

		}
	}
}

void APawnMissingCombat::MissingDistanceHandler()
{
	FoundMissings(); //It´s posible there were new spawn missing Drons (PNJ)
	RescueZoneCloser = false;
	if (ReturnDistanceToEvacuation() > 0 && ReturnDistanceToEvacuation() <= ReturnDistanceToPlayer())
	{
		
	    //Debug
		/*
		UE_LOG(LogTemp, Warning, TEXT("At MissingInAction   Dist min to Extraction: %f"), DistToRescue);
		UE_LOG(LogTemp, Warning,  TEXT("At APawnMissingCombat::MissingDistanceHandler  Dist PNJ to Extraction: %f"), ReturnDistanceToEvacuation());
		UE_LOG(LogTemp, Warning,  TEXT("At APawnMissingCombat::MissingDistanceHandler  Dist PNJ to player: %f"), ReturnDistanceToPlayer());
		*/
		RescueZoneCloser = true;
	}
}



void APawnMissingCombat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	


	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		return;
	}

	//If PNJ has an rescue zone closer than player pawn
	//PNJ move to rescue zone
	
	
	


	//if (ReturnDistanceToEvacuation()>0  &&  ReturnDistanceToEvacuation() <= ReturnDistanceToPlayer() )
	// Get From a timer Function -> MissingDistanceHandler() ->  Enable/Disable flag RescueZoneCloser
	//This  bool change every 1/2 seconds, improve perfomance because we do´t call two for loop every tick 
	if(RescueZoneCloser) 
	{
		
		if (RescueVolumeActors.Num() > 0) {
			
			FVector Translation = UKismetMathLibrary::GetForwardVector(
				RotateBase(RescueVolumeActors[0]->GetActorLocation()
				)
			);

			//Estamos moviento la maya padre pero no a los hijos
			//GetBaseMesh()->AddWorldOffset(Translation);
			GetCapsule()->AddWorldOffset(Translation);

			//UE_LOG(LogTemp, Warning, TEXT("At MissingInAction Tick  RescueVolumeActors move to Extraction"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("At MissingInAction Tick  RescueVolumeActors is not Founded or is nullptr"));
		}

		return;
	}
	
	
	


	
	
	//if player is Too Far  PNJ Doesn´t Move
	if (ReturnDistanceToPlayer() > TooFar)
	{
		return;
	}

	//if player is in rescue PNJ Doesn´t Move	
	if (PlayerPawn->GetIsInZoneRescue())
	{
		return;
    }

	//But allways Rotate to Player
	FVector	Translation = UKismetMathLibrary::GetForwardVector(
		RotateBase(PlayerPawn->GetActorLocation()
		)
	);	
	
	//FVector Translation = Velocity * 100 * DeltaTime;	

	
	
		if (ReturnDistanceToPlayer() >= DistToRescue)
		{
			//Near the player
			//UE_LOG(LogTemp, Error, TEXT("PNJ Pawn tank Move from: %s   to: %s "),*GetActorLocation().ToCompactString(),	*PlayerPawn->GetActorLocation().ToCompactString());
			//GetBaseMesh()->AddWorldOffset(Translation);
			//GetCapsule()->AddWorldOffset(Translation);

			AddActorWorldOffset(Translation, true);
		}
	
	

}



float APawnMissingCombat::ReturnDistanceToPlayer()
{
	// Check if we have a valid reference to the player.
	
	
	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn Not loade or none"));
		return 0.0f;
	}

	float Distance = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
	UE_LOG(LogTemp, Warning, TEXT("Distance drone to PlayerPawn: %f"), Distance);
	//float Distance = (PlayerPawn->GetBaseMesh()->GetComponentLocation() - GetBaseMesh()->GetComponentLocation()).Size();
	return Distance;



}

float APawnMissingCombat::ReturnDistanceToEvacuation()
{
	// Check if we have a valid reference to the player.
	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn Not loade or none"));
		return -1.0f;
	}
	
	
	float CurrentDist = 0.f; //(RescueVolumeActors[0]->GetActorLocation() - GetActorLocation()).Size();
	float Dist_Min_ToClose = 10000.0f;
	ARescueZone* RescueVolumeNearActor =nullptr;
	if (RescueVolumeActors.Num() > 0)
	{
		
		//if rescue zone near to drone is:
		for (auto RescueZone: RescueVolumeActors)
		{
			CurrentDist = (RescueZone->GetActorLocation() - GetActorLocation()).Size();

			if (CurrentDist <= Dist_Min_ToClose)
			{
				check(GEngine != nullptr);

				// Display a debug message for five seconds. 
				// The -1 "Key" value argument prevents the message from being updated or refreshed.			
				/*FString TempMessage = FString::Printf(TEXT("The Rescue zone: %s , is at: %f from dron"), *RescueZone->GetName(), CurrentDist);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempMessage);*/
				

				Dist_Min_ToClose = CurrentDist;
				RescueVolumeNearActor = RescueZone;
			}
		
		}

		CurrentDist = (RescueVolumeNearActor->GetActorLocation() - GetActorLocation()).Size();	
		
	  
	}
	
	RescueVolumeActors[0] = RescueVolumeNearActor;
	return CurrentDist;

}


void APawnMissingCombat::ShowFX()
{

	// Particle System
	if (MissingParticle)
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			MissingParticle,
			GetBaseMesh()->GetRelativeLocation()  //Get ActorLocation()
		);

	// Audio FX
	if (MissingSound)
		UGameplayStatics::PlaySoundAtLocation(
			this,
			MissingSound,
			GetBaseMesh()->GetRelativeLocation()  //GetActorLocation()
		);



}

void APawnMissingCombat::ShowNotCarriedFX()
{

	// Particle System
	if (MissingNotCarriedParticle) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			MissingNotCarriedParticle,
			this->GetCapsule()->GetRelativeLocation()  //
		);
	}
		
		


	// Audio FX
	if (MissingNotCarriedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			MissingNotCarriedSound,
			GetBaseMesh()->GetRelativeLocation()  //GetActorLocation()
		);
	}
		



}



void APawnMissingCombat::OnBOver_Rescue(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Error, TEXT("Overlap trigger"));
	
	if ( OtherActor && OtherActor != this   )
	{
		//UE_LOG(LogTemp, Error, TEXT("Overlap with OtherActor ..."));		
		
		//if (OtherActor->ActorHasTag(TEXT("player"))) 
		//{
		//	UE_LOG(LogTemp, Error, TEXT("PNJ Detected Hastag player missing rescated"));
		//	ShowFX();
		//	//Destroy();
		//	return;
		//}

		//if (OtherActor->ActorHasTag("enemy")) return;

		APawnTank* Player = Cast<APawnTank>(OtherActor);
		
		
		//If Player Pawn Is RescueZone -> PNJ Drones doesn´t move and Player Pawn doesnt take any drones  
		//Si el player está en una zona de rescate ignora a Missing in Actions
		//no debe Subir al jeep a los drones ( destruir actores) 
	    // if(	Player -> getISInZoneRescue() ) return;

		//if Player
		if(Player == OtherActor  && !Player->GetIsInZoneRescue())  //Cast<APawnTank>(OtherActor)
		{
			
			
			if(Player->GetMissingInAction()>= Player->GetMaxPassenger())
			{

				UE_LOG(LogTemp, Error, TEXT("PNJ Actor player Not Posible  rescates to missing because IT MaxPassenger in cabine "));

				ShowNotCarriedFX();

				return;
			}

			ShowFX();

			UE_LOG(LogTemp, Error, TEXT("PNJ  Actor player missing rescated"));
			Player->HealingMe(); //Haz una sobrecarga
			Player->HealingMe(15);
			

			//PlayerPawn->FuryMe();//No funciona

			//Aumento de Missin in In Action in Jeep
			Player->SetMissingInAction(1);
			
			Player->Save();  // upgrade the missil 
			
							//Aumento Score
			ATankGameModeBase* GameMode = Cast<ATankGameModeBase>( UGameplayStatics::GetGameMode(GetWorld()) );
			
			//Player->GetPureHealthComponent()->GameModeRef->SetScore(5);
			if(GameMode!=nullptr)
				GameMode->SetScore(5);
			else 
				UE_LOG(LogTemp, Error, TEXT("Game mode is nulptr or none"));

			//ShowFX();
			//K2_DestroyActor();
			HandleDestruction();
		  	
			
			
		}
		else
		{
			if(!OtherActor) UE_LOG(LogTemp, Error, TEXT("No detected OtherActor Look Collision Setup "));
		}
		

	

	    
	}
}




FRotator APawnMissingCombat::RotateBase(FVector LookAtTarget)
{

	FVector StartLocation = GetBaseMesh()->GetComponentLocation();

	FRotator BaseMeshRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation,
		FVector(LookAtTarget.X, LookAtTarget.Y, GetBaseMesh()->GetComponentLocation().Z));

	// Rotate Turret.

	GetBaseMesh()->SetWorldRotation(BaseMeshRotation);

	return BaseMeshRotation;
}


bool APawnMissingCombat::GetEvacFlag() const
{
	return EvacFlag;
}


void APawnMissingCombat::SetEvacFlag(bool newFlag)
{
	EvacFlag = newFlag;
}





void APawnMissingCombat::HandleDestruction()
{
	// Call base pawn class HandleDestruction to play effects.
	Super::HandleDestruction();
	// Hide AI. TODO 

	

	Destroy();
	//- Create new function to Do this. 

}



//Futuros usos

void APawnMissingCombat::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag("enemy")) return;
		if (OtherActor->ActorHasTag("boss")) return;

		if(ARescueZone* RescueClosedZone = Cast<ARescueZone>(OtherActor))
		{
			ShowFX();
			HandleDestruction();
		}
		

		//Delay

	   //   Destroy();
	}
}

void APawnMissingCombat::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag("enemy")) return;
		if (OtherActor->ActorHasTag("boss")) return;
		//ShowFX();

		//Delay

	   //   Destroy();
	}
}






