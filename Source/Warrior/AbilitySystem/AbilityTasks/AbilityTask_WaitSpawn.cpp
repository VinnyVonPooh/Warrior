// Groundhog & Drake All Rights Reserved.

#include "AbilityTask_WaitSpawn.h"

#include "AbilitySystemComponent.h"
#include "Warrior/WarriorDebugHelper.h"

UAbilityTask_WaitSpawn* UAbilityTask_WaitSpawn::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag,
                                                                 TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn,
                                                                 int NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius,
                                                                 const FRotator& SpawnRotation)
{
	auto* Node = NewAbilityTask<UAbilityTask_WaitSpawn>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	Node->CachedSpawnRotation = SpawnRotation;

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
	Debug::Print("Event Received");
	EndTask();
}
