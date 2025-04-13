// Victor J. Pasin Rguez All Rights Reserved


#include "GulagZone.h"

//Project
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/Pawns/PawnMissingCombat.h"


//Engine and 
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"

//Components

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"


//Libraries
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShake.h"


// Sets default values
AGulagZone::AGulagZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Walls
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	//BaseMesh->SetupAttachment(RootComponent);
	RootComponent = WallMesh;

	//Front
	FrontMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Mesh"));
	FrontMesh->SetupAttachment(GetWallMesh());

	//Cristals
	CritalLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Cristal Mesh"));
	CritalLeftMesh->SetupAttachment(GetFrontMesh());
	CritalRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Cristal Mesh"));
	CritalRightMesh->SetupAttachment(GetFrontMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	//const float Disper = DroneDispersion * 1.f;
	FMath::RandInit(DroneDispersion);
}

// Called when the game starts or when spawned
void AGulagZone::BeginPlay()
{
	Super::BeginPlay();
	
}


void AGulagZone::GulagZoneDestroyed()
{
	HandleDestruction();
}

void AGulagZone::HandleDestruction()
{	

	const int32 DronesUnleash = FMath::RandRange(2, 5);
	for (size_t i = 0; i < DronesUnleash; i++)
	{
		SpawnMissing();
	}	

	NiagaraStart();
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);

	Destroy();

}

void AGulagZone::SpawnMissing()
{

	if ( !MissingInActionClass )
	{
		UE_LOG(LogTemp,Error, TEXT("No Drone Missing Class loaded at Gulag Bleurpint Gulag Settings"))
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	
        //Spawn in random Loc and Rot
	const float x = FMath::RandRange(-4.f * DroneDispersion, 4.f * DroneDispersion);
	const float y = FMath::RandRange(-4.f * DroneDispersion, 4.f* DroneDispersion);
	const FVector  PawnMissingSpawnLocation = GetWallMesh()->GetComponentLocation() + FVector(x, y, 100.f);
	const FRotator PawnMissingSpawnRotator = GetWallMesh()->GetComponentRotation() + FRotator(0.f, 0.f, (x + y) / 10);;
	
	APawnMissingCombat* PawnMissed = GetWorld()->SpawnActor<APawnMissingCombat>(
		MissingInActionClass,
		PawnMissingSpawnLocation,
		PawnMissingSpawnRotator
		, SpawnParams
		);
	
	if (!PawnMissed)
	{		
		UE_LOG(LogTemp, Error, TEXT("At RescueZone::SpawnMissing  PawnMissed is not Spawned or is nullptr"));
		return;
	}

	MissingInActions.Emplace(PawnMissed);


}

UHealthComponent* AGulagZone::GetPureHealthComponent() const
{
	return HealthComponent;
}



