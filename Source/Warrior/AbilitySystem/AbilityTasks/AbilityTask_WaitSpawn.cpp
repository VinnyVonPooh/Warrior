// Groundhog & Drake All Rights Reserved.

#include "AbilityTask_WaitSpawn.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Warrior/WarriorDebugHelper.h"
#include "Warrior/Characters/WarriorEnemyCharacter.h"

UAbilityTask_WaitSpawn* UAbilityTask_WaitSpawn::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag,
																 TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
																 int NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	auto* Node = NewAbilityTask<UAbilityTask_WaitSpawn>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_WaitSpawn::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawn::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	Delegate.Remove(DelegateHandle);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawn::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull())) {
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(), //
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded));
	} else {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawn::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	auto* World = GetWorld();

	if (!LoadedClass || !World) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}

		EndTask();
		return;
	}

	TArray<AWarriorEnemyCharacter*> SpawnedEnemies;
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < CachedNumToSpawn; ++i) {

		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
		RandomLocation += FVector(0.f, 0.f, 150.f);

		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		auto* SpawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);
		if (SpawnedEnemy) {
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates()) {
		if (!SpawnedEnemies.IsEmpty()) {
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		} else {
			DidNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
		}
	}

	EndTask();
}
