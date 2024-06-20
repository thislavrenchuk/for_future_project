// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Bubble.generated.h"

class ABaseMultiplierEnemy;

UCLASS()
class HUNTER_API ABubble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABubble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly means this value cannot be changed during runtime
	TSubclassOf<ABaseMultiplierEnemy> MultiplierClass;

	/************************************************************************/
	/* PARTICLE EFFECT                                                      */
	/************************************************************************/
	UPROPERTY(EditAnywhere, Category="Particle Effect")
	UParticleSystem* ParticleEffect;
	UPROPERTY(EditAnywhere, Category="Particle Effect")
	float ParticleDeltaTime = 1.0;
	UPROPERTY(EditAnywhere, Category="Particle Effect")
	float ParticleInterpSpeed = 1.0;

	void Multiply();

	bool CheckSpawnLocationLineTraceToPlayer(FVector SpawnLocation);

	bool CheckSpawnInFrontOfPlayer(FVector SpawnLocation);
};
