// Groundhog & Drake All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UWarriorWidgetBase;

UCLASS()
class WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawWidget(UWarriorWidgetBase* InWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawWidgetIfAny();

private:
	TArray<UWarriorWidgetBase*> EnemyDrawnWidjets;
};
