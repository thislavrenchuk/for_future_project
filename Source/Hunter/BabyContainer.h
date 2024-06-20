#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BabyContainer.generated.h"

UCLASS()
class HUNTER_API ABabyContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABabyContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
};