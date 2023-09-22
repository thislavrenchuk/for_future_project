// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"

// Sets default values
ABow::ABow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a root Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	// Create a Skeletal mesh Component & setup attachement to the Root
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(Root);
	
}

void ABow::Shoot()
{
	// Particle effect
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		ShootEffect, 
		SkeletalMeshComponent->GetSocketLocation("ProjectileSocket"),
		SkeletalMeshComponent->GetSocketRotation("ProjectileSocket")
	);
	
	// Spawn projectile
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ActorSpawnParameters.bNoFail = false;
	ActorSpawnParameters.Owner = this->GetOwner(); // Set owner of projectile to Character
	ActorSpawnParameters.Instigator = NULL;

    Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		SkeletalMeshComponent->GetSocketTransform("ProjectileSocket"),
		ActorSpawnParameters
	);

	if (Projectile)
	{
		Projectile->SetUp();
	}
}

// Called when the game starts or when spawned
void ABow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

