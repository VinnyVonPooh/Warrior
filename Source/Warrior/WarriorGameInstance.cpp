// Groundhog & Drake All Rights Reserved.


#include "WarriorGameInstance.h"

TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InTag)
{
	for (const auto& GameLevelSet : GameLevelSets) {
		if (!GameLevelSet.IsValid()) {
			continue;
		}

		if (GameLevelSet.LevelTag == InTag) {
			return GameLevelSet.Level;
		}
	}
}