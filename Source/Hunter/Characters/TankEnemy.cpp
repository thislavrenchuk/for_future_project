// Fill out your copyright notice in the Description page of Project Settings.


#include "TankEnemy.h"
#include "Engine/World.h"

// Sets default values
ATankEnemy::ATankEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATankEnemy::BeginPlay()
{
    Super::BeginPlay();
}

float ATankEnemy::GetAttackDamage() const 
{
    return TankAttackDamage;
}

float ATankEnemy::GetMaxHealth() const
{
    return TankMaxHealth;
}