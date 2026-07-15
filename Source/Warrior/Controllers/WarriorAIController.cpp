// Groundhog & Drake All Rights Reserved.


#include "WarriorAIController.h"

#include "Navigation/CrowdFollowingComponent.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
}