// Groundhog & Drake All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "HeroUIComponent.generated.h"


UCLASS()
class WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;
};
