// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputConfigData.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Set Timeline Curve
    auto XCurve = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Script/Engine.CurveFloat'/Game/Data/C_AimZoom_X.C_AimZoom_X'"));
	if (XCurve.Object)
	{
		XFloatCurve = XCurve.Object;
	}
    auto YCurve = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Script/Engine.CurveFloat'/Game/Data/C_AimZoom_Y.C_AimZoom_Y'"));
	if (YCurve.Object)
	{
		YFloatCurve = YCurve.Object;
	}
    auto ZCurve = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Script/Engine.CurveFloat'/Game/Data/C_AimZoom_Z.C_AimZoom_Z'"));
	if (ZCurve.Object)
	{
		ZFloatCurve = ZCurve.Object;
	}
    auto FOVCurve = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Script/Engine.CurveFloat'/Game/Data/C_AimZoom_FOV.C_AimZoom_FOV'"));
	if (FOVCurve.Object)
	{
		FOVFloatCurve = FOVCurve.Object;
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    // Set Health at full
    Health = MaxHealth;

    // Set up Timeline Component
    FOnTimelineFloat onTimelineCallback;
    FOnTimelineEventStatic onTimelineFinishedCallback;
        
    if (XFloatCurve != NULL && YFloatCurve != NULL)
    {
        auto Timeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"), EObjectFlags::RF_NoFlags, nullptr, false, nullptr);
        MyTimeline = Timeline;

        MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
        this->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
        MyTimeline->SetNetAddressable(); // This component has a stable name that can be referenced for replication
        MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
        MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
        MyTimeline->SetLooping(false);
        MyTimeline->SetTimelineLength(1.0f);
        MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength); // TL_TimelineLength or TL_LastKeyFrame
        MyTimeline->SetPlaybackPosition(0.0f, false);

        //Add the float curve to the timeline and connect it to your timelines's interpolation function
        onTimelineCallback.BindUFunction(this, FName{TEXT("TimelineCallback")});
        onTimelineFinishedCallback.BindUFunction(this, FName{TEXT("TimelineFinishedCallback")});
        MyTimeline->AddInterpFloat(XFloatCurve, onTimelineCallback, FName{TEXT("XZoom-PropertyName")}, FName{TEXT("XZoom-TrackName")}); //FIXME not necessary
        
        MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

        MyTimeline->RegisterComponent();
    }

    // Get Spring Arm Component
    SpringArmComponent = Cast<USpringArmComponent>(this->GetComponentByClass(USpringArmComponent::StaticClass()));

    // Get Camera Component
    CameraComponent = Cast<UCameraComponent>(this->GetComponentByClass(UCameraComponent::StaticClass()));

    // Spawn a child Bow Actor with the Character Actor
    Crossbow = GetWorld()->SpawnActor<ABow>(WeaponClass);
	Crossbow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("CrossbowSocket"));
    Crossbow->SetOwner(this);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (MyTimeline != NULL)
    {
        MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
    }
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Get the player controller
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* PlayerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    // Clear out existing mapping, and add our mapping
    PlayerSubSystem->ClearAllMappings();
    PlayerSubSystem->AddMappingContext(InputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// Bind the actions
	EnhancedInputComponent->BindAction(InputConfigData->InputMove, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
	EnhancedInputComponent->BindAction(InputConfigData->InputLook, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
	EnhancedInputComponent->BindAction(InputConfigData->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction(InputConfigData->InputZoom, ETriggerEvent::Started, this, &ABaseCharacter::ZoomIn);
    EnhancedInputComponent->BindAction(InputConfigData->InputZoom, ETriggerEvent::Completed, this, &ABaseCharacter::ZoomOut);
    EnhancedInputComponent->BindAction(InputConfigData->InputShoot, ETriggerEvent::Completed, this, &ABaseCharacter::Shoot);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    DamageToApply = FMath::Min(Health, DamageToApply);
    Health -= DamageToApply;
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Health: %f"), Health); //TODO remove after debug

    if (Health == 0)
    {
        IsDead = true;
    }
    
    return DamageToApply;
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D MoveValue = Value.Get<FVector2D>();
        const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
 
        // Forward/Backward direction
        if (MoveValue.Y != 0.f)
        {
            // Get forward vector
            const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
            AddMovementInput(Direction, MoveValue.Y);
        }
        // Right/Left direction
        if (MoveValue.X != 0.f)
        {
            // Get right vector
            const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
            AddMovementInput(Direction, MoveValue.X);
        }
    }
}
 
void ABaseCharacter::Look(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D LookValue = Value.Get<FVector2D>();
        if (LookValue.X != 0.f) AddControllerYawInput(LookValue.X);
        if (LookValue.Y != 0.f) AddControllerPitchInput(LookValue.Y);
    }
}

void ABaseCharacter::Shoot(const FInputActionValue& Value)
{
    if (Controller != nullptr && Crossbow != nullptr)
    {
        Crossbow->Shoot();
    }
}

void ABaseCharacter::ZoomIn(const FInputActionValue& Value)
{
    if (Controller != nullptr && Crossbow != nullptr && SpringArmComponent != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ZoomIn function kicked off"));
        PlayTimeline();
    }
}

void ABaseCharacter::ZoomOut(const FInputActionValue& Value)
{
    if (Controller != nullptr && Crossbow != nullptr && SpringArmComponent != nullptr)
    {
        MyTimeline->Reverse();
        isZoomingIn = false;
        isZoomingOut = true;
    }
}

void ABaseCharacter::TimelineCallback(float interpolatedVal)
{
    // This function is called for every tick in the timeline.
    // UE_LOG(LogTemp, Warning, TEXT("I'm in TimelineCallback, with interpolatedValX: %f, and interpolatedValY: %f"), interpolatedValX, interpolatedValY); //TODO remove
    float position = MyTimeline->GetPlaybackPosition();
    if (SpringArmComponent != nullptr)
    {
        SpringArmComponent->SocketOffset.X = XFloatCurve->GetFloatValue(position);
        SpringArmComponent->SocketOffset.Y = YFloatCurve->GetFloatValue(position);
        SpringArmComponent->SocketOffset.Z = ZFloatCurve->GetFloatValue(position);
    }
    
    // if (CameraComponent != nullptr) 
    // {
    //     CameraComponent->FieldOfView = FOVFloatCurve->GetFloatValue(position);
    // }
}

void ABaseCharacter::TimelineFinishedCallback()
{
    // This function is called when the timeline finishes playing.
    isZoomingIn = false;
    isZoomingOut = false;
}

void ABaseCharacter::PlayTimeline()
{
    if (MyTimeline != NULL)
    {
        if (isZoomingOut) 
        {
            MyTimeline->Play();
        } else {
            MyTimeline->PlayFromStart();
        }
        isZoomingIn = true;
        isZoomingOut = false;    
    }
}