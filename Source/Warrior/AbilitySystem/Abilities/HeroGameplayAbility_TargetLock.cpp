// Groundhog & Drake All Rights Reserved.

#include "HeroGameplayAbility_TargetLock.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Warrior/WarriorDebugHelper.h"
#include "Warrior/Characters/WarriorHeroCharacter.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
													  const FGameplayAbilityActivationInfo ActivationInfo,
													  const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
												 bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	TArray<FHitResult> BoxTraceHits;

	const auto* HeroCharacter = GetHeroCharacterFromActorInfo();
	UKismetSystemLibrary::BoxTraceMultiForObjects(HeroCharacter,					 //
												  HeroCharacter->GetActorLocation(), //
												  HeroCharacter->GetActorLocation() +
													  HeroCharacter->GetActorForwardVector() * BoxTraceDistance,				   //
												  TraceBoxSize / 2.f,															   //
												  HeroCharacter->GetActorForwardVector().ToOrientationRotator(),				   //
												  BoxTraceChannel,																   //
												  false,																		   //
												  TArray<AActor*>(),															   //
												  bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None, //
												  BoxTraceHits,																	   //
												  true);

	for (const auto& TraceHit : BoxTraceHits) {
		if (AActor* HitActor = TraceHit.GetActor()) {
			if (HitActor != HeroCharacter) {
				AvailableActorsToLock.AddUnique(HitActor);

				Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}