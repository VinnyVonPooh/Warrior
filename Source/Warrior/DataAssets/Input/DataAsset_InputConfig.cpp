// Groundhog & Drake All Rights Reserved.


#include "DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag)
{
	for (const auto& InputActionConfig : NativeInputActions) {
		if (InputActionConfig.InputTag == InInputTag) {
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}