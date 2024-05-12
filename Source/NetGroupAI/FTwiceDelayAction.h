// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Engine/Engine.h"
#include "LatentActions.h"
/**
 * 
 */
class NETGROUPAI_API FTwiceDelayAction:public FPendingLatentAction
{
public:
	float TotalTime;
	float TimeRemaining;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	DELAY_EXEC& execRef;
	bool bHalfTriggered = false;

	FTwiceDelayAction(float Duration, const FLatentActionInfo& LatentInfo,DELAY_EXEC& exec)
		:TotalTime(Duration),
		 TimeRemaining(Duration),
		 ExecutionFunction(LatentInfo.ExecutionFunction),
		 OutputLink(LatentInfo.Linkage),
		 CallbackTarget(LatentInfo.CallbackTarget),
		 execRef(exec)
		 {

		 }

	virtual void UpdateOperation(FLatentResponse& Response)override
	{
		TimeRemaining -= Response.ElapsedTime(); 

		//已经过了一半时间
		if (TimeRemaining < TotalTime / 2.0f && !bHalfTriggered)
		{
			execRef = DELAY_EXEC::HalfExec;
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);//回调
			bHalfTriggered = true;
		}
		else if (TimeRemaining < 0.0f)//已经超过整体时间
		{
			execRef = DELAY_EXEC::CompletedExec;
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);//回调
			Response.DoneIf(TimeRemaining < 0.0f);// 结束UpdateOperation循环并销毁FPendingLatentAction派生类对象
		}
	}


	~FTwiceDelayAction();
};
