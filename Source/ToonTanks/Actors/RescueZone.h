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

	

private:

	

	void SpawnMissing();

	FTimerHandle TimerSpawnHandler;

public:	

	APawnTank* Player;
	int32 TotalMissings;
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	ATankGameModeBase* GameModeRef;

	//Inicio de Maniobra de Extracción
	UFUNCTION()
	 void OnOverBegin_EvacStart(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	//Segunda de la maniobra de extracción
	UFUNCTION()
	 void OnOverBegin_EvacEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);


	UFUNCTION()
	void OnOverEnd_Player(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	 void OnCompHit_EvacEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetExtractionShipMesh() const { return ExtractionShipMesh; }

	/**Returns the WhereToSpawn subobject */
	FORCEINLINE UBoxComponent* GetEvacZone() const { return EvacZone; }

};
