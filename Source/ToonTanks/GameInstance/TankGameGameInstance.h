// Víctor J. Pasín Rguez All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TankGameGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UTankGameGameInstance : public UGameInstance
{
	GENERATED_BODY()


	UTankGameGameInstance(const FObjectInitializer & ObjectInitializer);


	virtual void Init() override;

private:

	int32 LevelID;

	int32 MissinInActionRescued;

	

	int32 GI_Score;

	//int32 MissigInActionTransported;

protected:

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Loop")//, meta = (AllowPrivateAccess = "true"))
		FString StartLevel = TEXT("Level1");

	void UpdateLevelID();

	void UpdateLevelID(int32 Delta);

	void UpdateScore(int32 Delta);

	UFUNCTION(BlueprintPure)
	int32 GetMissingInActionRescued() const;

	UFUNCTION(BlueprintPure)
	int32 GetLevelID() const;

	//int32 GetMissigInActionTransported() const;

	UFUNCTION(BlueprintPure)
		int32 GetGI_Score() const;







};
