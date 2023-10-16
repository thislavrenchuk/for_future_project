// Fill out your copyright notice in the Description page of Project Settings.


#include "Bubble.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Hunter/Characters/BaseMultiplierEnemy.h"

// Sets default values
ABubble::ABubble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
    
	// Add box-collision
	if (!BoxCollisionComponent)
	{
		// Use a box as a simple collision representation.
		BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		BoxCollisionComponent->SetupAttachment(Root);
	}

	// Create a Static Mesh Component & setup attachement to the RootComponent
	if (!StaticMeshComponent)
	{
		StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMeshComponent->SetupAttachment(Root);
	}
	
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner is empty"));
	} else 
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner is populated"));
	}

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

// Called when the game starts or when spawned
void ABubble::BeginPlay()
{
	Super::BeginPlay();

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
	Multiply();

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

void ABubble::Multiply()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to Multiply()")); //TODO remove
	// Spawn a new Multiplier somewhere nearby
    FNavLocation SpawnLocation;
    UWorld* World = GEngine->GetWorldContexts()[0].World();
	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() returns nullptr")); //TODO remove
	}
    UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FVector OriginalEnemyLocation = GetActorLocation();
    UE_LOG(LogTemp, Warning, TEXT("OriginalEnemyLocation: %s"), *OriginalEnemyLocation.ToString());

	if (!NavigationSystem)
	{
		return;
	}
	
    
    bool bSuccess = NavigationSystem->GetRandomReachablePointInRadius(OriginalEnemyLocation, 400.0f, SpawnLocation);
    if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("bSuccess was false"));//TODO remove
	}
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation: %s"), *SpawnLocation.Location.ToString()); //TODO remove

    if (bSuccess) 
    {
        // FRotator SpawnRotation = this->GetOwner()->GetActorRotation(); // Rotate towards the player
		FRotator SpawnRotation = GetOwner()->GetActorRotation();
        FActorSpawnParameters EnemySpawnParameters;
        EnemySpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        EnemySpawnParameters.bNoFail = true;

        ABaseMultiplierEnemy* MyTwin = GetWorld()->SpawnActor<ABaseMultiplierEnemy>(MultiplierClass, SpawnLocation.Location, SpawnRotation, EnemySpawnParameters);
        //TODO: might need to also get the AIController to possess the new enemy
    }
}