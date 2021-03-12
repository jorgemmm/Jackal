
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

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

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
	
	//GetBaseMesh()->OnComponentHit.AddDynamic(this, &APawnMissingCombat::OnHit_Rescue);	
	//GetBaseMesh()->OnComponentBeginOverlap.AddDynamic(this, &APawnMissingCombat::NotifyActorEndOverlap);

	
	

}

void APawnMissingCombat::BeginPlay()
{
	Super::BeginPlay();
	
	
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

}


void APawnMissingCombat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	


	if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
	{
		return;
	}

	//Debes dcirle al missing in action que si tiene un rescue zon más cerca de 300 
	//que no persigas al player sino que vayas al cento del rescue zone

	UE_LOG(LogTemp, Warning, TEXT("At MissingInAction Tick  Dist min to Extraction: %f"), DistToRescue);
	UE_LOG(LogTemp, Warning, TEXT("At MissingInAction Tick  Dist current to Extraction: %f"), ReturnDistanceToEvacuation());
	UE_LOG(LogTemp, Warning, TEXT("At MissingInAction Tick  Dist current to player: %f"), ReturnDistanceToPlayer());


	if (ReturnDistanceToEvacuation()>0  &&  ReturnDistanceToEvacuation() <= ReturnDistanceToPlayer() )//1000.f ) //(DistToRescue)) // ReturnDistanceToPlayer())
	{
		//UE_LOG(LogTemp, Error, TEXT("Extraction!!!"))
		if (RescueVolumeActors.Num() > 0) {
			
			FVector Translation = UKismetMathLibrary::GetForwardVector(
				RotateBase(RescueVolumeActors[0]->GetActorLocation()
				)
			);

			//Estamos moviento la maya padre pero n a los hijos
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
	
	
	


	//UE_LOG(LogTemp, Warning, TEXT("PNJ Move Dist : %f"), ReturnDistanceToPlayer());
	
	//Si player está demasiado lejos nos quedamos quietos
	if (ReturnDistanceToPlayer() > TooFar)
	{
		return;
	}

	//Si player está en zona de rescate no vamos hacia player
	if (PlayerPawn->GetIsInZoneRescue())
	{
		return;
    }

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

	//float Distance = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
	float Distance = (PlayerPawn->GetBaseMesh()->GetComponentLocation() - GetBaseMesh()->GetComponentLocation()).Size();
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARescueZone::StaticClass(), FoundActors);
	//WhereToEvac = 

	for (auto Actor : FoundActors)
	{
		ARescueZone* RescueVolumeActor = Cast<ARescueZone>(Actor);
		if (RescueVolumeActor)
		{
			RescueVolumeActors.AddUnique(RescueVolumeActor);
		}
	}

	float Distance = 10000.0f;
	if (RescueVolumeActors.Num() > 0)
	{
		Distance = (RescueVolumeActors[0]->GetExtractionShipMesh()->GetComponentLocation() - GetBaseMesh()->GetComponentLocation()).Size();
	}
		
		
	

	return Distance;

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
		////atta
		//UGameplayStatics::SpawnEmitterAttached(MissingNotCarriedParticle,
		//	GetBaseMesh(), )
		//	
		//);
		


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

			UE_LOG(LogTemp, Error, TEXT("PNJ  Actor player missing rescated"));
			Player->HealingMe(); //Haz una sobrecarga
			Player->HealingMe(15);
			

			//PlayerPawn->FuryMe();//No funciona

			//Aumento de Missin in In Action in Jeep
			Player->SetMissingInAction(1);
			
			Player->Save();  // upgrade the missil 
			
							//Aumento Score
			//ATankGameModeBase* GameMode = Cast<ATankGameModeBase>( UGameplayStatics::GetGameMode(GetWorld()) );
			
			Player->GetPureHealthComponent()->GameModeRef->SetScore(5);
			//GameMode->SetScore(5);


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


void APawnMissingCombat::OnHit_Rescue(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor && (OtherActor != this))
	{
		//UE_LOG(LogTemp, Error, TEXT("Hit ..."));

		/*ARescueZone* RescueZone = Cast<ARescueZone>(OtherActor);
		UStaticMeshComponent* GetExtractionShipMesh = RescueZone->GetExtractionShipMesh();
		if(GetExtractionShipMesh)*/

			//UStaticMeshComponent* GetExtractionShipMesh = Cast<RescueZone->GetExtractionShipMesh()>(OtherActor);

		//Destroy();
	}
}

void APawnMissingCombat::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	//Not in use
	if (OtherActor && (OtherActor != this))
	{
		//UE_LOG(LogTemp, Error, TEXT("PNJ Move Dist : %f"), ReturnDistanceToPlayer());


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
		ShowFX();

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
		ShowFX();

		//Delay

	   //   Destroy();
	}
}


