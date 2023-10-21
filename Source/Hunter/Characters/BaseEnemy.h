// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "BaseEnemy.generated.h"

UCLASS()
class HUNTER_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	/************************************************************************/
	/* HEALTH                                                               */
	/************************************************************************/
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
	float Health;
	UPROPERTY(VisibleAnywhere)
	bool IsDead = false;

	/************************************************************************/
	/* ATTACK                                                               */
	/************************************************************************/
	UPROPERTY(EditAnywhere)
	float AttackDamage = 10;
	virtual void Attack();
	bool CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult);
protected:
	UPROPERTY(EditAnywhere)
	float MaxAttackRange = 100; // 1 metres
};
