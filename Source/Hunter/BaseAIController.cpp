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
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
    
}

void ABaseAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    // VERSION 2 - with dumb Behavior Tree
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // if (LineOfSightTo(PlayerPawn))
    // {
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    // } 
    // else 
    // {
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // }

    // VERSION 1 - without BehaviorTree
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
