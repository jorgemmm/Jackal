// Victor J. Pasin Rguez All Rights Reserved


#include "TargetSpawn.h"

// Sets default values
ATargetSpawn::ATargetSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

