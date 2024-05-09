// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a Static Mesh Component & setup attachement to the RootComponent
	if (!SkeletalMeshComponent && GetMesh() != nullptr)
	{
		SkeletalMeshComponent = GetMesh();
	}
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set Health at full
    Health = MaxHealth;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    // Is damage more than health remaining?
    float DamageToApplyCalc = FMath::Min(Health, DamageToApply);
    
    Health -= DamageToApplyCalc;

    // If Dead, apply approporiate changes
    if (Health <= 0.0f)
    {
        IsDead = true;
    }
	else 
	{
		IsHit = true;
	}

	return DamageToApply;
}

void ABaseEnemy::Attack()
{
	if (!IsDead) 
	{
		OnAttackEvent.Broadcast();
		FHitResult HitResult;
		FVector MyLocation = GetActorLocation();
		FRotator MyRotation = GetActorRotation();

		// Generate Line Trace
		bool HitSuccessful = CreateLineTrace(MyLocation, MyRotation, HitResult);		
		// Apply Damage
		AActor* HitActor = HitResult.GetActor();
		if (HitSuccessful && HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(AttackDamage, HitResult, -MyRotation.Vector(), nullptr);
			HitActor->TakeDamage(AttackDamage, DamageEvent, this->GetInstigatorController(), this);
		}
	}
}

bool ABaseEnemy::CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult)
{
	// Create a line trace 
	FVector LineTraceEnd = Location + Rotation.Vector() * MaxAttackRange;
	// Ignore yourself
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
	return bSuccess;
}

float ABaseEnemy::GetHealth()
{
	return Health;
}

float ABaseEnemy::GetAttackDamage() const {
	return AttackDamage;
}
