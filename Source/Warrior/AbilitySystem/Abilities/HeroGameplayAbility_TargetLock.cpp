// Groundhog & Drake All Rights Reserved.

#include "HeroGameplayAbility_TargetLock.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Warrior/WarriorDebugHelper.h"
#include "Warrior/Characters/WarriorHeroCharacter.h"
#include "Warrior/Controllers/WarriorHeroController.h"
#include "Warrior/Widgets/WarriorWidgetBase.h"

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
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty()) {
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockedActor) {
		DrawTargetLockWidget();
	} else {
		CancelTargetLockAbility();
	}
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
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget) {

		check(TargetLockWidgetClass);

		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);
		check(DrawnTargetLockWidget);
		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget) {
		DrawnTargetLockWidget->RemoveFromParent();
	}
}