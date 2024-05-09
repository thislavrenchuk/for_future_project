// Fill out your copyright notice in the Description page of Project Settings.


#include "BTask_Shoot.h"
#include "AIController.h"
#include "Hunter/Characters/BaseEnemy.h"

UBTask_Shoot::UBTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIOwner = OwnerComp.GetAIOwner(); 
    if (AIOwner == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    ABaseEnemy* OwnerAsBaseEnemy = Cast<ABaseEnemy>(AIOwner->GetPawn()); 
    if (OwnerAsBaseEnemy == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    OwnerAsBaseEnemy->Attack(); 

    return EBTNodeResult::Succeeded;
}
