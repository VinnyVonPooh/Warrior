// Groundhog & Drake All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "WarriorEnumTypes.h"
#include "Engine/LatentActionManager.h"

class WarriorCountDownAction : public FPendingLatentAction
{
public:
	WarriorCountDownAction(float InTotalCountTime, float InUpdateInterval, float& InOutRemainingTime,
						   EWarriorCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& FLatentInfo)
		: bNeedToCancel(false)
		, TotalCountDownTime(InTotalCountTime)
		, UpdateInterval(InUpdateInterval)
		, OutRemainingTime(InOutRemainingTime)
		, CountDownOutput(InCountDownOutput)
		, ExecutionFunction(FLatentInfo.ExecutionFunction)
		, OutputLink(FLatentInfo.Linkage)
		, CallbackTarget(FLatentInfo.CallbackTarget)
		, ElapsedInterval(0.f)
		, ElapsedTimeSinceStart(0.f)
	{
		
	}

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EWarriorCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
