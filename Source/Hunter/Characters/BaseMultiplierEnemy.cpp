// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMultiplierEnemy.h"
#include "Engine/World.h"

// Sets default values
ABaseMultiplierEnemy::ABaseMultiplierEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseMultiplierEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    // Spawn bubbles
	FActorSpawnParameters BubbleSpawnParameters;
	BubbleSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	BubbleSpawnParameters.bNoFail = false;
	BubbleSpawnParameters.Owner = this; 
	BubbleSpawnParameters.Instigator = NULL;

    LeftBubble = GetWorld()->SpawnActor<ABubble>(BubbleClass, BubbleSpawnParameters);
    LeftBubble->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftBubbleSocket"));
    LeftBubble->SetOwner(this);

    RightBubble = GetWorld()->SpawnActor<ABubble>(BubbleClass, BubbleSpawnParameters);
    RightBubble->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightBubbleSocket"));
    RightBubble->SetOwner(this);
}
