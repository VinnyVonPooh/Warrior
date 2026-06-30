// Groundhog & Drake All Rights Reserved.


#include "WarriorBaseCharacter.h"

#include "Components/SkeletalMeshComponent.h"

AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
}

