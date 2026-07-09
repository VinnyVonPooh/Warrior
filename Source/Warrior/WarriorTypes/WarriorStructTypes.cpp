// Groundhog & Drake All Rights Reserved.

#include "WarriorStructTypes.h"

#include "Warrior/AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

bool FWarriorHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}