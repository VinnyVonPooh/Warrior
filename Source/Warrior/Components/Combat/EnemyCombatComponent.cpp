// Groundhog & Drake All Rights Reserved.

#include "EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "Warrior/WarriorFunctionLibrary.h"
#include "Warrior/WarriorGameplayTags.h"
#include "Warrior/Characters/WarriorEnemyCharacter.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) {
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	// TODO:
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable =
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_Unblockable);

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable) {

		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	if (bIsValidBlock) {
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, WarriorGameplayTags ::Player_Event_SuccessfulBlock, Data);
	} else {
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Shared_Event_MeleeHit, Data);
	}
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	auto* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();
	check(OwningEnemyCharacter);

	auto* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	auto* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType) {
		case EToggleDamageType::LeftHand:
			LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
			break;
		case EToggleDamageType::RightHand:
			RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
			break;
		default:
			break;
	}

	if (!bShouldEnable) {
		OverlappedActors.Empty();
	}
}
