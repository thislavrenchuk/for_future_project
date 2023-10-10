// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMultiplierEnemy.h"

// Sets default values
ABaseMultiplierEnemy::ABaseMultiplierEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseMultiplierEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Spawn two Bubble Actors 
    Bubble1 = GetWorld()->SpawnActor<ABubble>();
    Bubble2 = GetWorld()->SpawnActor<ABubble>();
    Bubble1->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bubble1Socket"));
    Bubble2->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bubble2Socket"));
    Bubble1->SetOwner(this);
    Bubble2->SetOwner(this);
}

void ABaseMultiplierEnemy::Multiply()
{
    // Spawn a new Multiplier somewhere nearby
    FVector Origin = this->GetActorLocation();
    FNavLocation SpawnLocation;
    UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavigationSystem)
	{
		return;
	}

    bool bSuccess = NavigationSystem->GetRandomReachablePointInRadius(Origin, NewEnemySpawnRadius, SpawnLocation);
    
    if (bSuccess) 
    {
        FRotator SpawnRotation = this->GetActorRotation(); // Rotate towards the player
        ABaseMultiplierEnemy* MyTwin = GetWorld()->SpawnActor<ABaseMultiplierEnemy>(SpawnLocation, SpawnRotation);
        //TODO: might need to also get the AIController to possess the new enemy
    }

}