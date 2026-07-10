// Groundhog & Drake All Rights Reserved.

#include "WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Warrior/WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefencePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentHealth);
	}
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute()) {
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		// TODO::Notify UI

		// TODO::Handle character death
		if (NewCurrentHealth == 0.f) {
			
		}

		Debug::Print("OldHealth", OldHealth);
		Debug::Print("NewCurrentHealth", NewCurrentHealth);
		Debug::Print("DamageDone", DamageDone);
	}
}