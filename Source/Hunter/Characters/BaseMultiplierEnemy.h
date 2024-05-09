// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Math/UnrealMathUtility.h"
#include "Hunter/Bubble.h"
#include "BaseMultiplierEnemy.generated.h"

/**
 * A multiplier enemy spawns more enemies if one of the "Bubbles" or "Gas Chambers is hit"
 */
UCLASS()
class HUNTER_API ABaseMultiplierEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMultiplierEnemy();

	UPROPERTY(EditAnywhere)
	float NewEnemySpawnRadius = 300;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	ABubble* LeftBubble;

	UPROPERTY()
	ABubble* RightBubble;

	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly means this value cannot be changed during runtime
	TSubclassOf<ABubble> BubbleClass;

};
