// Fill out your copyright notice in the Description page of Project Settings.

#include  "HealthComponent.h"
//#include  "ToonTanks/Components/HealthComponent.h"
//Engine
#include  "ToonTanks/GameMode/TankGameModeBase.h"

//Pawns
#include "ToonTanks/Pawns/PawnMissingCombat.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTankEnemy.h"

//Utils
#include  "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	//MusicPlayedHealthy =   UGameplayStatics::CreateSound2D(this, HealthySound);
	////MusicPlayedHealthy->SetupAttachment(GetOwner()->getcapsule);
	//MusicPlayedLowHealthy = UGameplayStatics::CreateSound2D(this, LowHealthSound);
	//MusicPlayedDeathly =  UGameplayStatics::CreateSound2D(this, DeathlyHealthSound);


}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	
	Super::BeginPlay();
	
	//Se actualiza s i DefaultHealt se edita en el editor
	//Porque BeginPlay se ejecuta despu�s de empezar y toma los valores del editor
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


		if (Owner->ActorHasTag(TEXT("player")))
		{
			
			UE_LOG(LogTemp, Warning, TEXT("%s: is a Player  "), *Owner->GetName());
			/*if (!HealthySound)
			{
				UE_LOG(LogTemp, Error, TEXT(" Healthy Sound is none or nullptr in healthcomponent"));
				return;
			}
			UGameplayStatics::PlaySound2D(this, HealthySound);
			*/
			
			
		}

		if (Owner->ActorHasTag(TEXT("enemy")))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: is a Enemy  "), *Owner->GetName());
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
	
	UE_LOG(LogTemp, Warning, TEXT("Damage tank hero by %s") , *DamageCauser->GetName());
	
	if (Damage == 0 )//|| Health == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Damaging tank With zero value"));
		
		return;
	}
	

	if (Owner->ActorHasTag(TEXT("boss")))
	{
		/* Dañanado al boss */

		//Min 0 Max 100,  Add Damage
		float AmountDamage = Health -(Damage / 5.f);

		Health = FMath::Clamp(AmountDamage, 0.f, DefaultHealth);
		//UE_LOG(LogTemp, Warning, TEXT("%s: is a Boss  "), *Owner->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("And this Boss is receiveing %f Brute Damage"), Damage);
		//UE_LOG(LogTemp, Warning, TEXT("And this Boss is receiveing %f neto Damage"), AmountDamage);
		

	}
	else if (Owner->ActorHasTag(TEXT("player")))
	{
		//Player
		//Min 0 Max 100,  Add Damage

		APawnTank* PlayerPawn = Cast<APawnTank>(GetOwner());

		float AmountDamage = Health - Damage;

		if (PlayerPawn->GetMissingInAction() >= PlayerPawn->GetMaxPassenger())
		{
			AmountDamage = Health - (Damage / 5.f);

		}
		else if (PlayerPawn->GetMissingInAction() >= PlayerPawn->GetMaxPassenger() / 2)
		{
			AmountDamage = Health - (Damage / 2.f);

		}
		/*else
		{
			Health = FMath::Clamp(Health - Damage,  0.f, DefaultHealth);
		}*/

		Health = FMath::Clamp(AmountDamage, 0.f, DefaultHealth);





	}
	else if (Owner->ActorHasTag(TEXT("enemy")))
	{
		float AmountDamage = Health - (Damage / 2.f);

		Health = FMath::Clamp(AmountDamage, 0.f, DefaultHealth);

		//Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);


		//APawnTankEnemy* Enemy = Cast<APawnTankEnemy>(DamageCauser);

		//No se permite fuego amigo entre tanques.
		//if(Enemy) Health = FMath::Clamp(Health - 0.f, 0.f, DefaultHealth);

	}	
	else if (Owner->ActorHasTag(TEXT("turret")))
	{
		//Hero dañando a los tanques y torretas
		//APawnTank* PlayerPawn = Cast<APawnTank>(DamageCauser);
		//Turret o Enemytanks
		//Min 0 Max 100,  Add Damage
		//if(PlayerPawn) 
		Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	}
	else if (  Owner->ActorHasTag(TEXT("gulag")) || Owner->ActorHasTag(TEXT("carcel"))  )
	{
		//Actores que sin tags 

		float AmountDamage = Health - 2*Damage;

		Health = FMath::Clamp(AmountDamage, 0.f, DefaultHealth);
		
	
		
	}
	

	
	
	UE_LOG(LogTemp, Warning, TEXT("Actor or Pawn: %s"), *Owner->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Left Following Health: %f"), Health);

	//Info to GM if Player died.
	if ( Health <= 0  )
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


UAudioComponent* UHealthComponent::PlayMusic(USoundBase* PlaySound)
{
	
	

	/*if (MusicPlayedHealthy!=nullptr)    MusicPlayedHealthy->Stop();
	if (MusicPlayedLowHealthy!=nullptr) MusicPlayedLowHealthy->Stop();
	if (MusicPlayedDeathly!=nullptr)    MusicPlayedDeathly->Stop();*/
	
	return UGameplayStatics::SpawnSound2D(this, PlaySound);
	 
	
	
	
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


