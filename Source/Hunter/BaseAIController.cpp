// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    }
    
}

void ABaseAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // bool InLineOfSight = LineOfSightTo(PlayerPawn);
    // if (InLineOfSight)
    // {
    //     SetFocus(PlayerPawn);
    //     MoveToActor(PlayerPawn, AcceptanceRadius);
    //     // TODO: Attack
    // } else 
    // {
    //     ClearFocus(EAIFocusPriority::Gameplay);
    //     StopMovement();
    //     //TODO: Idle
    // }
    
}
