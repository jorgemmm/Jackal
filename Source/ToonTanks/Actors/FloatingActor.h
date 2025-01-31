﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class TOONTANKS_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();


	/** Return the mesh for the BaseMesh */
	FORCEINLINE UStaticMeshComponent* GetVisualMesh() const { return VisualMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float MovementScale = 20.f;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* VisualMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
