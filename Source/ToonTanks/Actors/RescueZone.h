// Víctor J. Pasín Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RescueZone.generated.h"




class UStaticMeshComponent;
class UBoxComponent;
class APawnMissingCombat;
class APawnTank;
class ATankGameModeBase;


UCLASS()
class TOONTANKS_API ARescueZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARescueZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ExtractionShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent*  EvacZone;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rescue Zone", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawnMissingCombat> MissingInActionClass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rescue Zone", meta = (AllowPrivateAccess = "true"))
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rescue Zone", meta = (AllowPrivateAccess = "true"))
		TArray<APawnMissingCombat*> MissingInActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gulag Settings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake>  DeathShake;

	

private:

	

	void SpawnMissing();

	//Change in child class: if turret or tank
	void HandleDestruction();

	FTimerHandle TimerSpawnHandler, TimerDestroyRescueZone;

	


public:	

	APawnTank* Player;
	int32 TotalMissings;
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	ATankGameModeBase* GameModeRef;

	//Inicio de Maniobra de Extracción
	/**When Player Arrive Rescue Zone Spawn Drones: Drones get down from jackal*/
	UFUNCTION()
	 void OnOverBegin_EvacStart(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	//Fin de la maniobra de extracción
	UFUNCTION()
	 void OnOverBegin_EvacEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);


	UFUNCTION()
	void OnOverEnd_Player(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetExtractionShipMesh() const { return ExtractionShipMesh; }

	/**Returns the WhereToSpawn subobject */
	FORCEINLINE UBoxComponent* GetEvacZone() const { return EvacZone; }

	/** Fire an Event at Blueprint to Active a niagara particle system */
	UFUNCTION(BlueprintImplementableEvent)
		void NiagaraStart();



	

};
