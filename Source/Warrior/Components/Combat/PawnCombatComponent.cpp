// Groundhog & Drake All Rights Reserved.

#include "PawnCombatComponent.h"

#include "Warrior/WarriorDebugHelper.h"
#include "Warrior/Items/Weapons/WarriorWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister,
                                                 bool bRegisterAsEquippedWeapon)
{
	check(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister));
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	if (bRegisterAsEquippedWeapon) {
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	const FString WeaponStr =
		FString::Printf(TEXT("weapon: %s, tag: %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debud::Print(WeaponStr);
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet)) {
		if (auto FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet)) {
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid()) {
		return nullptr;
	}
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}