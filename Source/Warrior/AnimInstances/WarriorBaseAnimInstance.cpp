// Groundhog & Drake All Rights Reserved.

#include "WarriorBaseAnimInstance.h"

#include "GameFramework/Pawn.h"
#include "Warrior/WarriorFunctionLibrary.h"

bool UWarriorBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner()) {
		return UWarriorFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}