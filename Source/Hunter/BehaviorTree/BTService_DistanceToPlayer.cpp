// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DistanceToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTService_DistanceToPlayer::UBTService_DistanceToPlayer()
{
    NodeName = TEXT("Update Distance To Player");
}

void UBTService_DistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr) {
        return;
    }
    AController* AIController = Cast<AController>(OwnerComp.GetOwner());
    if (AIController) { 
        if (AIController->GetPawn()) {
            FVector AILocation = AIController->GetPawn()->GetActorLocation();
            FVector PlayerLocation = PlayerPawn->GetActorLocation();
            float Distance = (AILocation - PlayerLocation).Size();             
            OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
        }
    }   
}
