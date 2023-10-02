// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
    DamageToApply = FMath::Min(Health, DamageToApply);
    
    Health -= DamageToApply;
    
    UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health); //TODO remove after debug

    // If Dead, apply approporiate changes
    if (Health <= 0.0f)
    {
        IsDead = true;
        // TODO: play dead animation

        // Detach controller from character
        DetachFromControllerPendingDestroy();
        // Switch off capsule collision
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void ABaseEnemy::Attack()
{
	// TODO: opportunity for some smarter AI
	// - for example, if low on health, will retreat temporarily
	if (!IsDead) 
	{
		FHitResult HitResult;
		FVector EnemyLocation = this->GetActorLocation();
		FRotator EnemyRotation = this->GetActorRotation();
		// Generate Line Trace
		bool HitSuccessful = CreateLineTrace(EnemyLocation, EnemyRotation, HitResult);
		
		AActor* HitActor = HitResult.GetActor();
		// Apply Damage
		if (HitSuccessful && HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(AttackDamage, HitResult, -Rotation.Vector(), nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, this->GetInstigatorController(), this);
		}

	}
}

bool ABaseEnemy::CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult)
{
	// Create a line trace 
	FVector LineTraceEnd = Location + Rotation.Vector() * MaxAttackRange;
	// Ignore yourself
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
	return bSuccess;
}

