// Fill out your copyright notice in the Description page of Project Settings.


#include "LatentFunctionLibrary.h"
#include "FTwiceDelayAction.h"

void ULatentFunctionLibrary::TwiceDelay(const UObject* WorldContextObject, DELAY_EXEC& exec, float Duration, struct FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();//获取LatentActionManager
		if (LatentActionManager.FindExistingAction<FTwiceDelayAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FTwiceDelayAction(Duration, LatentInfo, exec));
		}
	}

}
