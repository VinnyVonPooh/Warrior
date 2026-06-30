// Groundhog & Drake All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Warrior/DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserObject, typename CallbackFunk>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
							   UserObject* ContextObject, CallbackFunk Funk);
};

template <class UserObject, typename CallbackFunk>
void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag,
	ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunk Funk)
{
	check(InInputConfig);
	if (auto* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag)) {
		BindAction(FoundAction, TriggerEvent, ContextObject, Funk);
	}
}
