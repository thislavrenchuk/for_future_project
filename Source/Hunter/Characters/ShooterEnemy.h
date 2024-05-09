// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Math/UnrealMathUtility.h"
#include "ShooterEnemy.generated.h"

/**
 * Will contain logic specific to the animation requirements of the Dekker skeleton. 
 */
UCLASS()
class HUNTER_API AShooterEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterEnemy();

	virtual float GetAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere)
	float ShooterAttackDamage = 30;

};
