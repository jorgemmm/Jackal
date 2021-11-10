// Fill out your copyright notice in the Description page of Project Settings.


#include "RescueZone.h"

//Engine and 
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"

#include  "ToonTanks/GameMode/TankGameModeBase.h"
#include "ToonTanks/Actors/ProjectileBase.h"


//Components
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

//Pawns
#include "ToonTanks/Pawns/PawnMissingCombat.h"
#include "ToonTanks/Pawns/PawnTank.h"

//Utils
#include "Kismet/GameplayStatics.h"

//#include "PawnTank.h"
#define OUT

// Sets default values
ARescueZone::ARescueZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ExtractionShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Rescue Mesh"));
	
	
	RootComponent = ExtractionShipMesh;
	//ExtractionShipMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	
	
	
	ExtractionShipMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ExtractionShipMesh->OnComponentBeginOverlap.AddDynamic(this, &ARescueZone::OnOverBegin_EvacEnd);
	ExtractionShipMesh->OnComponentHit.AddDynamic(this, &ARescueZone::OnCompHit_EvacEnd);


	EvacZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Rescue Zone"));

	EvacZone->SetupAttachment(RootComponent);//GetGetGroundRescueMesh() );

	EvacZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//RescueZone->GetScaledBoxExtent();	
	EvacZone->OnComponentBeginOverlap.AddDynamic(this, &ARescueZone::OnOverBegin_EvacStart);	
	EvacZone->OnComponentEndOverlap.AddDynamic(this,   &ARescueZone::OnOverEnd_Player);

}

// Called when the game starts or when spawned
void ARescueZone::BeginPlay()
{
	Super::BeginPlay();


	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
}



void ARescueZone::OnOverBegin_EvacStart(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{



	if (OtherActor && OtherActor != this)
	{
		//If Overlap with Player Pawn
		APawnTank* PlayerPawn = Cast<APawnTank>(OtherActor);
		//Player = Cast<APawnTank>(OtherActor);
		

		//if (!PlayerPawn)
		if (!PlayerPawn)
		{
			//Los drones
			UE_LOG(LogTemp, Error, TEXT("At RescueZone  OnOverBegin Pawn is not Founded or is nullptr"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Debug: Player pawn is overlappingBegin rescue none"));
		
		
		//PlayerPawn->SetIsInZoneRescue();


		//Update in Player Pawn bIsInZoneRescue = true And Change Collisidon to ignore All
		//FTimerHandle DelayToPlayerExtraction;
		//GetWorld()->GetTimerManager().ClearTimer(DelayToPlayerExtraction);
		//
		//GetWorld()->GetTimerManager().SetTimer(DelayToPlayerExtraction,
		//	Player, //This
		//	&APawnTank::SetIsInZoneRescue,
		//	//&ARescueZone::ResetPlayer,
		//	0.5f
		//);

		//Spawn 4 - 6 enemyTanks 


		//Check Missing in action Flag=Evac  
		//( Crear tb returndistancetoevaczone in MissininAction)
		//Debes d3cirle al missing in action que si tiene un rescue zon más cerca de 300 
	    //que no persigas al player sino que vayas al cento del rescue zone
		//PlayerPawn si tiene cerca 0 overlap un rescue zone Disable 

		PlayerPawn->SetIsInZoneRescue();
		

		
		if	(PlayerPawn->GetMissingInAction()<=0){

			UE_LOG(LogTemp, Warning, TEXT("At RescueZone   Player Pawn hasn´t any passenger: %f"), PlayerPawn->GetMissingInAction());

			return;
		}

		
		while (PlayerPawn->GetMissingInAction() > 0)
		{
			SpawnMissing(); // nada más llegas descargamos a uno
			PlayerPawn->SetMissingInAction(-1);

			/*GetWorld()->GetTimerManager().SetTimer(
				TimerSpawnHandler,
				this,
				&ARescueZone::SpawnMissing,
				1.5f,
				false);*/
		}

		//GetWorld()->GetTimerManager().ClearTimer(TimerSpawnHandler);
		//se ejecuta 1 vez
		//SpawnMissing(); // nada más llegas descargamos a uno
		//PlayerPawn->SetMissingInAction(-1);
		
		/*

		if (TotalMissings > 0) {
		if (PlayerPawn->GetMissingInAction() > 0) {
			
		//Esto seolo se ejecuta una vez
			//PlayerPawn->SetMissingInAction(-1);

		GetWorld()->GetTimerManager().SetTimer(
				TimerSpawnHandler,
				this,
				&ARescueZone::SpawnMissing,
				1.5f,
				false		


			);

			//PlayerPawn->SetMissingInAction(-1);
		
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerSpawnHandler);
		}
		*/
		
		PlayerPawn->Save();
		

	}

	

}


void ARescueZone::OnOverEnd_Player(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && OtherActor != this)
	{
		
		//APawnTank* PlayerPawn = Cast<APawnTank>(OtherActor);

		Player = Cast<APawnTank>(OtherActor);


		//if (!PlayerPawn)
		if (!Player)
		{

			UE_LOG(LogTemp, Error, TEXT("At RescueZone OnOverEnd Pawn is not Founded or is nullptr"));
			return;
		}

		//Poner esto En &APawnTank::ResetIsInZoneRescue y quitar Return
		//PlayerPawn->GetCapsule()->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		
		
		UE_LOG(LogTemp, Warning, TEXT("Debug: Player pawn is overlappingEnd rescue none"));
		//Update in Player Pawn bIsInZoneRescue = false And Change Collisidon to Block All Dynamic
		FTimerHandle DelayToPlayerReady;

		GetWorld()->GetTimerManager().ClearTimer(DelayToPlayerReady);
		
		GetWorld()->GetTimerManager().SetTimer(DelayToPlayerReady,			
			Player, //This
			&APawnTank::ResetIsInZoneRescue,
			//&ARescueZone::ResetPlayer,
			5.0f
			);			
		  
	}
}


/**Repasar si produce crash*/
void ARescueZone::OnOverBegin_EvacEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//Debes dcirle al missing in action in tick que si tiene un rescue zone más cerca de 300 
	//que no persigas al player sino que vayas al cento del rescue zone
	if (OtherActor && OtherActor != this)
	{
		//If Overlap with Player Pawn or Missil
		APawnTank* PlayerPawn = Cast<APawnTank>(OtherActor);
		AProjectileBase* Missil = Cast<AProjectileBase>(OtherActor);
		//Return
		if (PlayerPawn || Missil)
		{
			return;
		}

		
		
		//Es más eficeciente así probar
		// 
		//APawnMissingCombat*  PawnExtraction = Cast<APawnMissingCombat>(OtherActor);
		//if (!PawnExtraction)
		//{
		//	return;
		//}
		////Score
		//if (!GameModeRef)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("ARescueZone has no reference to the GameMode Not score updated"));
		//	return;

		//}
		//if (!PawnExtraction)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Casting APawnMissingCombat Failing in ARescueZone "));
		//	return;
		//}
		//GameModeRef->ActorDied(PawnExtraction); 

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnMissingCombat::StaticClass(), OUT FoundActors);

		for (auto Actor : FoundActors)
		{

			//APawnMissingCombat* PawnExtraction = Cast<APawnMissingCombat>(OtherActor);
			APawnMissingCombat* PawnInMap = Cast<APawnMissingCombat>(Actor);

			if (PawnInMap == OtherActor) //Explicar en Twitch que faltaba esta línea
			{
				//Score
				if (!GameModeRef)
				{
					UE_LOG(LogTemp, Error, TEXT("ARescueZone has no reference to the GameMode Not score updated"));
					return;

				}
				if (!PawnInMap)
				{
					UE_LOG(LogTemp, Error, TEXT("Casting APawnMissingCombat Failing in ARescueZone "));
					return;
				}
				GameModeRef->ActorDied(PawnInMap); //Actor->Destroy();
			}

			
		
			

			//PawnExtraction->HandleDestruction(); Mejor en Game Mode (Encargado de spawn y destroy actor)
		}



	}
}



//Debug only
void ARescueZone::OnCompHit_EvacEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)// && OtherComp)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	
	}



}




void ARescueZone::SpawnMissing()
{

	//Spawn all missing in actions With 
	if (!MissingInActionClass)
	{
		UE_LOG(LogTemp, Error, TEXT("At RescueZone::SpawnMissing   MissingInActionClass is not Founded or is nullptr"));
		return;
	} 
	       
			//Vector  PawnMissingSpawnLocation = PlayerPlayer->GetBaseMesh()->GetComponentLocation();
			

			
			// Set the spawn parameters

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			const float x = FMath::RandRange(100.f, +400.f);
			const float y = FMath::RandRange(-100.f, -400.f);

			//if(x < 120)  x = FMath::RandRange(-100.f, -300.f);
			//if(y > -120) y = FMath::RandRange(100.f, +300.f);

			const FVector  PawnMissingSpawnLocation = GetExtractionShipMesh()->GetComponentLocation() + FVector(x, y, 100.f);
			const FRotator PawnMissingSpawnRotator  = GetExtractionShipMesh()->GetComponentRotation()+FRotator (0.f,0.f,(x+y)/10);;
			//PawnMissingSpawnLocation += FVector(x, y, 100.f);
			//PawnMissingSpawnLocation += FVector(200.f, 200.f, 100.f);
			//PawnMissingSpawnRotator += FRotator(x / 2, y / 2, 0.f);

			

			APawnMissingCombat* PawnMissed = GetWorld()->SpawnActor<APawnMissingCombat>(
				MissingInActionClass,
				PawnMissingSpawnLocation,
				PawnMissingSpawnRotator
				,SpawnParams
				);

			
			//MissingInActions.EmplaceAt(i, PawnMissed);
			if (!PawnMissed)
			{
				//Informa de error de Spawnes
				UE_LOG(LogTemp, Error, TEXT("At RescueZone::SpawnMissing  PawnMissed is not Spawned or is nullptr"));
				return;
			}
			
			
			    
			MissingInActions.Emplace(PawnMissed);

			
			


}



//Destroy missing in actions

// Tick function Called every frame
/*

void ARescueZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/


