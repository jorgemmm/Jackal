// Fill out your copyright notice in the Description page of Project Settings.


#include  "HealthComponent.h"
//Engine
#include  "ToonTanks/GameMode/TankGameModeBase.h"

//Pawns
#include "ToonTanks/Pawns/PawnMissingCombat.h"
#include "ToonTanks/Pawns/PawnTank.h"

//Utils
#include  "Kismet/GameplayStatics.h"

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
		

		if ( Owner->ActorHasTag(TEXT("boss")) )
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: is a Boss  "), *Owner->GetName() );
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Health component has no reference to the Owner"));
	}


	if (bIsDamaged)
	{

	}
	else 
	{

	}

	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	

	if (Damage == 0 || Health == 0)
	{
		return;
	}
	

	if (Owner->ActorHasTag(TEXT("boss")))
	{
		
		//Min 0 Max 100,  Add Damage
		float AmountDamage = Health -(Damage / 5.f);

		Health = FMath::Clamp(AmountDamage, 0.f, DefaultHealth);
		//UE_LOG(LogTemp, Warning, TEXT("%s: is a Boss  "), *Owner->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("And this Boss is receiveing %f Brute Damage"), Damage);
		//UE_LOG(LogTemp, Warning, TEXT("And this Boss is receiveing %f neto Damage"), AmountDamage);
		

	}
	else if(Owner->ActorHasTag(TEXT("player")))
	{
		//Player
		//Min 0 Max 100,  Add Damage

		APawnTank* PlayerPawn = Cast<APawnTank>(GetOwner());

		float AmountDamage = Health - Damage;

		if (PlayerPawn->GetMissingInAction() >= PlayerPawn->GetMaxPassenger())
		{
			AmountDamage = Health - (Damage / 5.f);			

		}else if (PlayerPawn->GetMissingInAction() >= PlayerPawn->GetMaxPassenger()/2 )
		{
			AmountDamage = Health - (Damage / 2.f);
			
		}
		/*else
		{
			Health = FMath::Clamp(Health - Damage,  0.f, DefaultHealth);
		}*/

		Health = FMath::Clamp( AmountDamage , 0.f, DefaultHealth);

		
	}
	else 
	{
		//Turret o Enemytanks
		//Min 0 Max 100,  Add Damage
		Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	}
	

	
	
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

	//bIsDamaged = true;


}

void UHealthComponent::Heal(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.f, DefaultHealth);
}



float UHealthComponent::GetHealth() const
{
	return Health;
}



bool UHealthComponent::GetbIsDamaged() const
{
	return bIsDamaged;
}


