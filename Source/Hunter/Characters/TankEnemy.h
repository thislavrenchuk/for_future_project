// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Math/UnrealMathUtility.h"
#include "TankEnemy.generated.h"

/**
 * Will contain logic specific to the animation requirements of the Dekker skeleton. 
 */
UCLASS()
class HUNTER_API ATankEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATankEnemy();

	virtual float GetAttackDamage() const;
	virtual float GetMaxHealth() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere)
	float TankAttackDamage = 20;

	UPROPERTY(EditDefaultsOnly)
	float TankMaxHealth = 300;
};
