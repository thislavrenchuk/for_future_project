// Fill out your copyright notice in the Description page of Project Settings.


#include "BTask_Shoot.h"
#include "AIController.h"
#include "BaseCharacter.h"

UBTask_Shoot::UBTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIOwner = OwnerComp.GetAIOwner(); // Get AIOwner
    if (AIOwner == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    ABaseCharacter* OwnerAsBaseCharacter = Cast<ABaseCharacter>(AIOwner->GetPawn()); // Cast the Pawn to BaseCharacter
    if (OwnerAsBaseCharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    OwnerAsBaseCharacter->ShootAsPawn(); // Shoot

    return EBTNodeResult::Succeeded;
}
