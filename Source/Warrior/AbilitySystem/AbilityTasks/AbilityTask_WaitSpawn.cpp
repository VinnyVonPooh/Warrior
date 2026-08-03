// Groundhog & Drake All Rights Reserved.


#include "AbilityTask_WaitSpawn.h"

UAbilityTask_WaitSpawn* UAbilityTask_WaitSpawn::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag,
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSpawn, int NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius,
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