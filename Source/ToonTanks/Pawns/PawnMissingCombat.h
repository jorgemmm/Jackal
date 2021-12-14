// Víctor J. Pasín Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
//#include "PawnTurret.h"  
#include "PawnMissingCombat.generated.h"

/**
 * 
 */


class APawnTank;
class USphereComponent;
class ARescueZone;

UCLASS()
class TOONTANKS_API APawnMissingCombat : public APawnBase
{
	GENERATED_BODY()

public:
	APawnMissingCombat();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

private:
	
	
	// EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Missing", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MissingParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Missing", meta = (AllowPrivateAccess = "true"))
	USoundBase* MissingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Missing", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MissingNotCarriedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Missing", meta = (AllowPrivateAccess = "true"))
	USoundBase* MissingNotCarriedSound;

	FVector Velocity;

	FRotator RotateBase(FVector LookAtTarget);

	APawnTank* PlayerPawn;


	float ReturnDistanceToPlayer();


	float ReturnDistanceToEvacuation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missing", meta = (AllowPrivateAccess = "true"))
		float DistToRescue = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missing", meta = (AllowPrivateAccess = "true"))
		float TooFar = 800.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Missing", meta = (AllowPrivateAccess = "true"))
		USphereComponent* ZoneToRescue;


	//Activamos tick cuando player pawn llegue a la zona de evac
	bool EvacFlag = false;

	FTimerHandle TimerMissingHandler;

	void FoundMissings();

	void MissingDistanceHandler();

	bool RescueZoneCloser = false;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

	/** The pickup to spawn*/
	/*UPROPERTY(EditAnywhere, Category = "Missing")
	TSubclassOf<ARescueZone> WhereToEvac;*/
	TArray<ARescueZone*> RescueVolumeActors;

	// overlap begin function
	

	void ShowFX();

	void ShowNotCarriedFX();


	UFUNCTION()
		void OnBOver_Rescue(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
	
	


	

	
	
	/**Returns the WhereToSpawn subobject */
	FORCEINLINE USphereComponent* GetZoneToRescue() const { return ZoneToRescue; }


	UFUNCTION(BlueprintPure)
		bool GetEvacFlag() const;
	
	UFUNCTION(BlueprintCallable)
		void SetEvacFlag(bool newFlag);


	/** Use in Future */
	UFUNCTION()
		void NotifyActorEndOverlap(AActor* OtherActor);

	UFUNCTION()
		void NotifyActorBeginOverlap(AActor* OtherActor);



	

};
