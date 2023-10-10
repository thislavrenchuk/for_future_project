// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Hunter/Bubble.h"
#include "BaseMultiplierEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HUNTER_API ABaseMultiplierEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMultiplierEnemy();

	void Multiply();

	UPROPERTY(EditAnywhere)
	float NewEnemySpawnRadius = 300;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	ABubble* Bubble1;

	UPROPERTY()
	ABubble* Bubble2;

};
