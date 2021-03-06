// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ATankGameModeBase;
class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float Health = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		bool bIsDamaged = false;
public:	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DefaultHealth = 100.0f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
	USoundBase* HealthySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
	USoundBase* LowHealthSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MusicAsDamage", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathlyHealthSound;


	UAudioComponent* PlayMusic(USoundBase* PlaySound);


	/*Deprecated a nivel de game mode y en blueprint con nodo DoOnce*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MusicAsDamage")//, meta = (AllowPrivateAccess = "true"))
	//UAudioComponent* MusicPlayerHealthy;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MusicAsDamage")//, meta = (AllowPrivateAccess = "true"))
	//UAudioComponent* MusicPlayerLowHealthy;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MusicAsDamage")//, meta = (AllowPrivateAccess = "true"))
	//UAudioComponent* MusicPlayerVeryHealthy;
	

	AActor* Owner;

	ATankGameModeBase* GameModeRef;

	UFUNCTION(BlueprintCallable)
	void Heal(  float Delta=10);

	UFUNCTION(BlueprintPure)
		float GetHealth() const;

	UFUNCTION(BlueprintPure)
		bool GetbIsDamaged() const;
};
