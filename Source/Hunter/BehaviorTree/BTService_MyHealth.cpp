// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MyHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Hunter/Characters/BaseEnemy.h"

UBTService_MyHealth::UBTService_MyHealth()
{
    NodeName = TEXT("Update My Health");
}

void UBTService_MyHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (OwnerComp.GetOwner())
    {
        AController* AIController = Cast<AController>(OwnerComp.GetOwner());
        if (AIController->GetPawn()) {
            ABaseEnemy* Enemy = Cast<ABaseEnemy>(AIController->GetPawn());  
            OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Enemy->GetHealth());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("in Service_MyHealth: Could not get Enemy..."));
            return;
        }
    }
}
