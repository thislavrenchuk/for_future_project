// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class HUNTER_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200.0f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

protected:
	virtual void BeginPlay() override;
	
public: 
	virtual void Tick(float DeltaSeconds) override;

};
