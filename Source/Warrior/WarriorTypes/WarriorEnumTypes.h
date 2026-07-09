#pragma once

#include "WarriorEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

UENUM(BlueprintType)
enum class EWarriorValidType : uint8
{
	Valid,
	Invalid
};