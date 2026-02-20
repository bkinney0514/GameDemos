// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"

#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	void RotateTurret(FVector LookAtTarget);

	void Fire();

	void HandleDestruction();

};
