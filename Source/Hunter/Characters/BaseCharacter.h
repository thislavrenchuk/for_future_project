// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Hunter/Bow.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReceiveDamageEvent);

UCLASS()
class HUNTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ShootAsPawn();

	UPROPERTY(BlueprintAssignable)
	FDeathEvent OnDeathEvent;
	UPROPERTY(BlueprintAssignable)
	FReceiveDamageEvent OnReceiveDamageEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputConfigData;

	UPROPERTY(BlueprintReadWrite)
	bool IsShooting = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsAiming = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsStabbing = false;
	UPROPERTY()
	bool isZoomingIn = false;
	UPROPERTY()
	bool isZoomingOut = false;

	/************************************************************************/
	/* TIMELINE                                                             */
	/************************************************************************/
	UPROPERTY()
    UTimelineComponent* MyTimeline;
    UPROPERTY()
    UCurveFloat* XFloatCurve;
	UPROPERTY()
    UCurveFloat* YFloatCurve;
	UPROPERTY()
    UCurveFloat* ZFloatCurve;
	UPROPERTY()
    UCurveFloat* FOVFloatCurve;
	UPROPERTY()
    TEnumAsByte<ETimelineDirection::Type> TimelineDirection; 
	UFUNCTION()
    void TimelineCallback(float val);
	UFUNCTION()
    void TimelineFinishedCallback();
    void PlayTimeline();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health;

private:

	UPROPERTY()
	ABow* Crossbow;

	UPROPERTY()
	USpringArmComponent *SpringArmComponent;

	UPROPERTY()
	UCameraComponent *CameraComponent;

	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly means this value cannot be changed during runtime
	TSubclassOf<ABow> WeaponClass;

	/************************************************************************/
	/* ACTION                                                               */
	/************************************************************************/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Stab(const FInputActionValue& Value);
	void ZoomIn(const FInputActionValue& Value);
	void ZoomOut(const FInputActionValue& Value);

	/************************************************************************/
	/* HEALTH STATE                                                         */
	/************************************************************************/
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	bool IsDead = false;

	UPROPERTY(EditAnywhere)
	float StabDamage = 50;

};
