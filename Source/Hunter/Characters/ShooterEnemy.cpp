// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemy.h"
#include "Engine/World.h"

// Sets default values
AShooterEnemy::AShooterEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterEnemy::BeginPlay()
{
    Super::BeginPlay();
}

float AShooterEnemy::GetAttackDamage() const 
{
    return ShooterAttackDamage;
}