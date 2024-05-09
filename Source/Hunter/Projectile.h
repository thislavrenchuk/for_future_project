// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Projectile.generated.h"

UCLASS()
class HUNTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void SetUp();

	void Fly(FRotator Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(EditAnywhere)
	float MaxRange = 5000; // 50 metres

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// Projectile material
	UPROPERTY(VisibleDefaultsOnly)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UPROPERTY(EditAnywhere)
	float Damage = 50;

	void GetProjectileTransform(FVector &Location, FRotator &Rotation);

	void CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult);

};
