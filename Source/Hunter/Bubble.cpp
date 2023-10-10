// Fill out your copyright notice in the Description page of Project Settings.


#include "Bubble.h"
#include "Hunter/Characters/BaseMultiplierEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABubble::ABubble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABubble::BeginPlay()
{
	Super::BeginPlay();

	// Our root component will be a sphere that reacts to physics
    USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    Root = SphereComponent;
    SphereComponent->InitSphereRadius(40.0f);
    SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create and position a mesh component so we can see where our sphere is
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComponent->SetupAttachment(Root);
    StaticMeshComponent->SetStaticMesh(StaticMeshFile);
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(0.8f));

	// Create a particle system that we can activate or deactivate
    // ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
    // ParticleSystemComponent->SetupAttachment(SphereVisual);
    // ParticleSystemComponent->bAutoActivate = false;
    // ParticleSystemComponent->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
    // static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
    // if (ParticleAsset.Succeeded())
    // {
    //     ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    // }
}

// Called every frame
void ABubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABubble::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// When an arrow hits the Bubble, it should (1) spawn another Multiplier enemy AND (2) burst/die
	
	// 1. Spawn Multiplier upon being hit
	// 1.1 Cast Owner to BaseMultiplierEnemy
	ABaseMultiplierEnemy* Enemy = Cast<ABaseMultiplierEnemy>(GetOwner());
	// 1.2 Spawn Multiplier
	Enemy->Multiply();

	// 2. Burst and die
	// 2.1 Detach controller from character
	// DetachFromControllerPendingDestroy(); // Not required as not ACharacter but AActor
	// 2.2 Switch off capsule collision
	StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	this->SetActorEnableCollision(false);
	// 2.3 Play the burst animation, or become invisible
	StaticMeshComponent->SetVisibility(false);

	return DamageToApply;
}