#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameFramework/DamageType.h"
#include "StabNotify.generated.h"

UCLASS()
class HUNTER_API UStabNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	TSubclassOf<UDamageType> StabDamageType;

protected:
    float StabDamage = 100;
};