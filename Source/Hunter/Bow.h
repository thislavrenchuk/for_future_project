// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Bow.generated.h"

UCLASS()
class HUNTER_API ABow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABow();

	void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ShootEffect;

	UPROPERTY()
	AProjectile* Projectile;

	// EditDefaultsOnly means this value cannot be changed during runtime
	UPROPERTY(EditDefaultsOnly) 
	TSubclassOf<AProjectile> ProjectileClass;
};
