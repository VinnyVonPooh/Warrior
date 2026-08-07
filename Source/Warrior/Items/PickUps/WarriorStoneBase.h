// Groundhog & Drake All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarriorPickUpBase.h"
#include "WarriorStoneBase.generated.h"

class UGameplayEffect;
class UWarriorAbilitySystemComponent;

UCLASS()
class WARRIOR_API AWarriorStoneBase : public AWarriorPickUpBase
{
	GENERATED_BODY()

public:
	void Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;

	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
													 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
													 const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStoneConsumed"))
	void BP_OnStoneConsumed();
};
