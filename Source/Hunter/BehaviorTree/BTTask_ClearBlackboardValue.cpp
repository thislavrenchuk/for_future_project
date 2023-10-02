// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearBlackboardValue.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
    NodeName = TEXT("Clear Blackboard Value"); // sets a human-readable name in the Unreal BT UI
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;
}
