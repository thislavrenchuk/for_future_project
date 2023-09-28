// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "CollisionQueryParams.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->SetRootComponent(Root);

	// Add Box-collision
	if (!BoxCollisionComponent)
	{
		// Use a box as a simple collision representation.
		BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		BoxCollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Event called when component hits something.
		// BoxCollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
		BoxCollisionComponent->SetupAttachment(Root);
	}

	// Create a Projectile Movement Component (do SetUpdatedComponent(), otherwise default is Root)
	if(!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
		ProjectileMovementComponent->InitialSpeed = 500.0f;
		ProjectileMovementComponent->MaxSpeed = 500.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = false;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	}

	// Create a Static Mesh Component & setup attachement to the RootComponent
	if (!StaticMeshComponent)
	{
		StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMeshComponent->SetupAttachment(Root);
	}

	this->SetUp();

}

void AProjectile::SetUp()
{
	FVector Location; 
	FRotator Rotation;
	this->GetProjectileTransform(Location, Rotation);
	// Do not proceed if not correct location
	if (fabs(Location.X - 0) < 0.05f && fabs(Location.Y - 0) < 0.05f && fabs(Location.Z - 0) < 0.05f) return;

	// Create line trace to give damage
	FHitResult HitResult;
	CreateLineTrace(Location, Rotation, HitResult);

	// Set up the correct transform for the arrow
	FVector SpawnLocation = GetActorLocation();
	FVector HitPointLocation = HitResult.ImpactPoint; // same as HitResult.Location;
	FVector LineTraceEnd = Location + Rotation.Vector() * MaxRange;
	FRotator Direction = FRotationMatrix::MakeFromX(HitPointLocation - SpawnLocation).Rotator();
	SetActorRotation(Direction);
	
	// Fly 
	Fly(Direction);
}


void AProjectile::GetProjectileTransform(FVector &Location, FRotator &Rotation)
{
	// Get owner controller
	APawn* OwnerPawn = Cast<APawn>(this->GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;

	// Get player viewpoint
	OwnerController->GetPlayerViewPoint(Location, Rotation);
}

void AProjectile::CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult)
{
	// Create a line trace 
	FVector LineTraceEnd = Location + Rotation.Vector() * MaxRange;
	// Ignore certain actors
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
	if (bSuccess) 
	{
		//TODO: apply Damage if actor hit
		AActor* HitActor = HitResult.GetActor();

		if (HitActor != nullptr) 
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, -Rotation.Vector(), nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, this->GetOwner()->GetInstigatorController(), this);
		}
	}
}

void AProjectile::Fly(FRotator Direction)
{
	// Set projectile's velocity = direction * speed
	ProjectileMovementComponent->Velocity = Direction.Vector() * ProjectileMovementComponent->MaxSpeed;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

