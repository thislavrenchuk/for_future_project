// Fill out your copyright notice in the Description page of Project Settings.


#include "Bubble.h"

#include "Hunter/Characters/BaseMultiplierEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Navigation/NavQueryFilter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Projectile.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "math.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "DrawDebugHelpers.h"


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
	
}

// Called when the game starts or when spawned
void ABubble::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner is empty")); //TODO fix this
	} else 
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner is populated"));
	}

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
	// 1.1 Spawn Multiplier
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

bool ABubble::CheckSpawnInFrontOfPlayer(FVector SpawnLocation)
{
	// First vector is the player forward vector
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector FirstVector = PlayerPawn->GetActorForwardVector();
	// Second vector is the direction vector from player to target enemy location
	FVector SecondVector = SpawnLocation - PlayerPawn->GetActorLocation();
	SecondVector.Normalize();
	// Workout the dot product
	double DotProduct = FVector::DotProduct(FirstVector, SecondVector);
	// DotProduct >= 0 if the enemy is in front of player
	// and DotProduct < 0 if the enemy is behind the player
	return DotProduct >= 0;
}

void ABubble::Multiply()
{
		// Spawn a new Multiplier somewhere nearby
    FNavLocation SpawnLocation;
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FVector OriginalEnemyLocation = GetActorLocation();
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!NavigationSystem)
	{
		return;
	}
	
    bool bFoundGoodSpot = false;
    bool bSuccessfullyGenerated = false;
	bool bSpawnLocationInFrontOfPlayer = false;
	bool bNoOverlapWithOriginalEnemy = false;
	bool bNoOverlapWithPlayer = false;
	
	do
	{
		bSuccessfullyGenerated = NavigationSystem->GetRandomReachablePointInRadius(OriginalEnemyLocation, 500.0f, SpawnLocation);
    	// Check that it's visible to player
		bFoundGoodSpot = CheckSpawnLocationLineTraceToPlayer(SpawnLocation.Location);
		// Check that it's in front of the player
		bSpawnLocationInFrontOfPlayer = CheckSpawnInFrontOfPlayer(SpawnLocation.Location);
		// Check that it's not overlapping with original enemy 
		double DistanceBwOriginalEnemyAndTwin = (OriginalEnemyLocation - SpawnLocation.Location).SizeSquared();
		bNoOverlapWithOriginalEnemy = labs(DistanceBwOriginalEnemyAndTwin) >= 100000;
		// Check that it's not overlapping with player 
		double DistanceBwPlayerAndTwin = (PlayerPawn->GetActorLocation() - SpawnLocation.Location).SizeSquared();
		bNoOverlapWithPlayer = labs(DistanceBwPlayerAndTwin) >= 100000;
	} while (!bFoundGoodSpot || !bSuccessfullyGenerated || !bSpawnLocationInFrontOfPlayer || !bNoOverlapWithPlayer || !bNoOverlapWithOriginalEnemy);

	// If successful, spawn new enemy
	FActorSpawnParameters EnemySpawnParameters;
	EnemySpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	EnemySpawnParameters.bNoFail = true;
	// Spawn an actor that will fall in an arch to where a new Enemy will spawn	
	AActor* Sphere = GetWorld()->SpawnActor<AActor>(this->GetActorLocation(), this->GetActorRotation(), EnemySpawnParameters);
	if (ParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(ParticleEffect, Sphere->GetRootComponent(), NAME_None, Sphere->GetActorLocation(), Sphere->GetActorRotation(), EAttachLocation::SnapToTarget, false, EPSCPoolMethod::AutoRelease);
		FMath::VInterpTo(Sphere->GetActorLocation(), SpawnLocation.Location, ParticleDeltaTime, ParticleInterpSpeed);
		Sphere->Destroy();
	}

	// Spawn Enemy
	ABaseMultiplierEnemy* MyTwin = GetWorld()->SpawnActor<ABaseMultiplierEnemy>(MultiplierClass, SpawnLocation.Location, GetOwner()->GetActorRotation(), EnemySpawnParameters);
    }

bool ABubble::CheckSpawnLocationLineTraceToPlayer(FVector SpawnLocation)
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	FHitResult HitResult;
	// Get Player Pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerPawn->GetController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	
	// Set custom params
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner()); 
	// Create line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, SpawnLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
	// Check if hit actor is the enemy
	TArray<FHitResult> InHits;
	
	bool LocationIsVisibleToPlayer = false;
	if (HitResult.GetNumBlockingHits(InHits) == 0)
	{
		LocationIsVisibleToPlayer = true;
	}
	return LocationIsVisibleToPlayer;
}
