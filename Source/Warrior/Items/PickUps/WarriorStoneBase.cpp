// Groundhog & Drake All Rights Reserved.


#include "WarriorStoneBase.h"

#include "Warrior/WarriorGameplayTags.h"
#include "Warrior/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Warrior/Characters/WarriorHeroCharacter.h"

void AWarriorStoneBase::Consume(UWarriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	auto* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, AbilitySystemComponent->MakeEffectContext());

	BP_OnStoneConsumed();
}

void AWarriorStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* OverlappedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor)) {
		OverlappedHeroCharacter->GetWarriorAbilitySystemComponent()->TryActivateAbilityByTag(
			WarriorGameplayTags::Player_Ability_PickUp_Stones);
	}
}