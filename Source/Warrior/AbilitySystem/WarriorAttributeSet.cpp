// Groundhog & Drake All Rights Reserved.

#include "WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Warrior/WarriorDebugHelper.h"
#include "Warrior/WarriorFunctionLibrary.h"
#include "Warrior/WarriorGameplayTags.h"
#include "Warrior/Components/UI/HeroUIComponent.h"
#include "Warrior/Components/UI/PawnUIComponent.h"
#include "Warrior/Interfaces/PawnUIInterface.h"

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
	if (!CachedPawnUIInterface.IsValid()) {

		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
		check(CachedPawnUIInterface.IsValid());
	}

	const auto* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	check(PawnUIComponent);

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentHealth);

		if (const auto* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent()) {
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute()) {
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (GetCurrentHealth() == 0.f) {
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Death);
		}
	}
}