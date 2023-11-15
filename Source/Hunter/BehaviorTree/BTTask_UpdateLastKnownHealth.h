// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateLastKnownHealth.generated.h"

/**
 * 
 */
UCLASS()
class HUNTER_API UBTTask_UpdateLastKnownHealth : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_UpdateLastKnownHealth();

protected: 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
