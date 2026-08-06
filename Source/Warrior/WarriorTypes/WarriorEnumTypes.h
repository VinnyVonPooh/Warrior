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

UENUM(BlueprintType)
enum class EWarriorSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EWarriorCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};