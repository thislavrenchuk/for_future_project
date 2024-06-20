#include "Engine/DamageEvents.h"
#include "StabNotify.h"

void UStabNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    // do some damage 
    // while animation is playing check if damage has been dealt
    // if socket ArrowTip touches enemy space, deal damage
    FVector ArrowTipLocation = MeshComp->GetSocketLocation("ArrowTip");
    FVector ArrowBaseLocation = MeshComp->GetSocketLocation("ArrowEnd");
    FVector DamageAngle = (ArrowBaseLocation - ArrowTipLocation);
    StabDamageType = UDamageType::StaticClass();

    // Ignore certain actors
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(MeshComp->GetOwner());

    // Create a line trace
    FHitResult HitResult;
    bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, ArrowTipLocation, ArrowBaseLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
    
    if (bSuccess) 
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor != nullptr) 
        {
            FPointDamageEvent DamageEvent(StabDamage, HitResult, DamageAngle, nullptr);
            HitActor->TakeDamage(StabDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());
        }
    }

}