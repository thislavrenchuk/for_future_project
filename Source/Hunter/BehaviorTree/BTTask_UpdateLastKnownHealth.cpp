// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateLastKnownHealth.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Hunter/Characters/BaseEnemy.h"

UBTTask_UpdateLastKnownHealth::UBTTask_UpdateLastKnownHealth()
{
    NodeName = TEXT("Update LK Health");
}

EBTNodeResult::Type UBTTask_UpdateLastKnownHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetOwner())
    {
        AController* AIController = Cast<AController>(OwnerComp.GetOwner());
        if (AIController->GetPawn()) {
            ABaseEnemy* Enemy = Cast<ABaseEnemy>(AIController->GetPawn());  
            OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Enemy->GetHealth());
            return EBTNodeResult::Succeeded;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UBTTask_UpdateLastKnownHealth: Could not GetPawn..."));
            return EBTNodeResult::Failed;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_UpdateLastKnownHealth: Could not GetOwner..."));
        return EBTNodeResult::Failed;
    }
}
