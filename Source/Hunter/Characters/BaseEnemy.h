// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"
#include "BaseEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEvent);

UCLASS()
class HUNTER_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	UPROPERTY(BlueprintAssignable)
	FAttackEvent OnAttackEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();
	virtual void Attack();
	virtual float GetAttackDamage() const;

private:

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(EditAnywhere)
	float Health;

	/************************************************************************/
	/* ATTACK                                                               */
	/************************************************************************/
	UPROPERTY(EditAnywhere)
	float AttackDamage = 10;
	bool CreateLineTrace(FVector Location, FRotator Rotation, FHitResult &HitResult);

protected:
	UPROPERTY(EditAnywhere)
	float MaxAttackRange = 10000; // 100 metres

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsHit = false;
};
