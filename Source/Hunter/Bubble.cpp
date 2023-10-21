// Fill out your copyright notice in the Description page of Project Settings.


#include "Bubble.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Navigation/NavQueryFilter.h"
#include "Hunter/Characters/BaseMultiplierEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Projectile.h"
#include "Math/Vector.h"

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
		UE_LOG(LogTemp, Warning, TEXT("GetOwner is empty")); //TODO fix this
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


bool ABubble::CheckSpawnInFrontOfPlayer(FVector SpawnLocation)
{
	// First vector is the player forward vector
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector FirstVector = PlayerPawn->GetActorForwardVector();
	UE_LOG(LogTemp, Warning, TEXT("DEBUG: FirstVector: %s"), *FirstVector.ToString());
	// Second vector is the direction vector from player to target enemy location
	FVector SecondVector = SpawnLocation - PlayerPawn->GetActorLocation();
	SecondVector.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("DEBUG: SecondVector: %s"), *SecondVector.ToString());
	// Workout the dot product
	double DotProduct = FVector::DotProduct(FirstVector, SecondVector);
	UE_LOG(LogTemp, Warning, TEXT("DEBUG: DotProduct: %d"), DotProduct);
	
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
	if (!NavigationSystem)
	{
		return;
	}

	bool bFoundGoodSpot = false;
	bool bSuccessfullyGenerated = false;
	bool bSpawnLocationInFrontOfPlayer = false;
	// bool bRenderedInCamera = false;
	
	do
	{
		UE_LOG(LogTemp, Warning, TEXT("DEBUG: Looking for good SpawnLocation..."));
		bSuccessfullyGenerated = NavigationSystem->GetRandomReachablePointInRadius(OriginalEnemyLocation, 400.0f, SpawnLocation);
		UE_LOG(LogTemp, Warning, TEXT("DEBUG: bSuccessfullyGenerated: %s"), (bSuccessfullyGenerated ? TEXT("true") : TEXT("false")));
		// Check that it's visible to player
		bFoundGoodSpot = CheckSpawnLocationLineTraceToPlayer(SpawnLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("DEBUG: bFoundGoodSpot: %s"), (bFoundGoodSpot ? TEXT("true") : TEXT("false")));
		// Chack that it's in front of the player
		bSpawnLocationInFrontOfPlayer = CheckSpawnInFrontOfPlayer(SpawnLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("DEBUG: bSpawnLocationInFrontOfPlayer: %s"), (bSpawnLocationInFrontOfPlayer ? TEXT("true") : TEXT("false")));
		// TODO check that it's not overlapping with original multiplier 
	} while (!bFoundGoodSpot || !bSuccessfullyGenerated || !bSpawnLocationInFrontOfPlayer);

	// If successful, spawn new enemy
	UE_LOG(LogTemp, Warning, TEXT("DEBUG: Spawning new Enemy..."));
	FActorSpawnParameters EnemySpawnParameters;
	EnemySpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	EnemySpawnParameters.bNoFail = true;
	ABaseMultiplierEnemy* MyTwin = GetWorld()->SpawnActor<ABaseMultiplierEnemy>(MultiplierClass, SpawnLocation, GetOwner()->GetActorRotation(), EnemySpawnParameters);
	//TODO: might need to also get the AIController to possess the new enemy
    
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

// bool ABubble::CheckSpawnLocationIsVisibleInCamera(FVector SpawnLocation)
// {
// 	//Spawn invisible actor in SpawnLocation
// 	FActorSpawnParameters EnemySpawnParameters;
// 	EnemySpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
// 	EnemySpawnParameters.bNoFail = true;

// 	// AProjectile* InvisibleActor = (AProjectile*) GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnLocation, EnemySpawnParameters);
// 	// UStaticMeshComponent* InvisibleActorStaticMesh = Cast<UStaticMeshComponent>(InvisibleActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	
// 	// Spawn Actor
// 	ABaseMultiplierEnemy* MyTwin = GetWorld()->SpawnActor<ABaseMultiplierEnemy>(MultiplierClass, SpawnLocation, GetOwner()->GetActorRotation(), EnemySpawnParameters);
// 	// Get Skeletal Mesh
// 	USkeletalMeshComponent* TwinsSkeletalMeshComponent = MyTwin->GetMesh();
// 	if (TwinsSkeletalMeshComponent != nullptr)
// 	{
// 		TwinsSkeletalMeshComponent->SetVisibility(false);
// 		UE_LOG(LogTemp, Warning, TEXT("DEBUG: SetVisibility to false before check"));
// 	} 
// 	// Get Primitive Component
// 	UPrimitiveComponent* TwinsPrimitiveComponent = Cast<UPrimitiveComponent>(MyTwin->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	
// 	float lastRendered = 0.0f;
// 	if (TwinsPrimitiveComponent != nullptr)
// 	{
// 		lastRendered = TwinsPrimitiveComponent->GetLastRenderTimeOnScreen();
// 	} 

// 	// Check if it was rendered in the last 1 second & is therefore on Screen
// 	float lastAcceptableOnScreenTime = 1.0f;
// 	bool bSuccessfulSpawn = (lastRendered != 0.0f && lastRendered <= lastAcceptableOnScreenTime); // TODO this is always true because lastRendered is -1000.00000
// 	if (bSuccessfulSpawn && TwinsSkeletalMeshComponent != nullptr)
// 	{
// 		// TwinsSkeletalMeshComponent->SetVisibility(true);
// 		UE_LOG(LogTemp, Warning, TEXT("DEBUG: SetVisibility is true"));
// 	} else
// 	{
// 		// Destroy the newly spawned Actor
// 		MyTwin->DetachFromControllerPendingDestroy(); // Not required as not ACharacter but AActor
// 		// 2.2 Switch off capsule collision
// 		// TwinsSkeletalMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
// 		MyTwin->SetActorEnableCollision(false);
// 	}

// 	return bSuccessfulSpawn;
// }