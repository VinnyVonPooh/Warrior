// Groundhog & Drake All Rights Reserved.

#include "WarriorAbilitySystemComponent.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) {
		return;
	}
	for (const auto& AbilitySpec : GetActivatableAbilities()) {
		if (!AbilitySpec.DynamicAbilityTags.HasTag(InInputTag)) {
			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) {
		return;
	}
}