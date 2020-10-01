// Fill out your copyright notice in the Description page of Project Settings.


#include  "HealthComponent.h"
#include  "ToonTanks/GameMode/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	//Para valores default de clase 
	//Se carga con los valores de la instancia del objeto.
	//pero no actualiza valores de editor
	//Health = DefaultHealth; 
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	
	Super::BeginPlay();
	
	//Se actualiza s i DefaultHealt se edita en el editor
	//Porque BeginPlay se ejecuta después de empezar y toma los valores del editor
	Health = DefaultHealth;
	// ...
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Health component has no reference to the Owner"));
	}

	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	

	if (Damage == 0 || Health == 0)
	{
		return;
	}

	//Min 0 Max 100,  Add Damage
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), *Owner->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Left Following Health: %f"), Health);

	//Info to GM if Player died.
	if (Health <= 0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(Owner);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Health component has no reference to the GameMode"));
		}
	}



}

void UHealthComponent::Heal(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.f, DefaultHealth);
}



