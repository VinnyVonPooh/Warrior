// Groundhog & Drake All Rights Reserved.

#include "WarriorHeroLinkedAnimLayer.h"

#include "WarriorHeroAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UWarriorHeroAnimInstance* UWarriorHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UWarriorHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}