// Groundhog & Drake All Rights Reserved.


#include "WarriorBaseCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Warrior/AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Warrior/AbilitySystem/WarriorAttributeSet.h"

AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>("WarriorAbilitySystemComponent");
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>("WarriorAttributeSet");
}

UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}

void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (WarriorAbilitySystemComponent) {
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

