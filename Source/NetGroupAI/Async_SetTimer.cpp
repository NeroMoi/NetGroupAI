// Fill out your copyright notice in the Description page of Project Settings.


#include "Async_SetTimer.h"
#include"Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogAsyncAction);

//Storage all UAsync_SetTimer instance for destroy
static TMap<FString, UAsync_SetTimer*> TimerHandleInstMap;


UAsync_SetTimer::UAsync_SetTimer()
{
	WorldContextObject = nullptr;

	UE_LOG(LogAsyncAction, Log, TEXT("UAsync_SetTimer::UAsync_SetTimer(): Async_SetTimer object [%s] is being created."), *this->GetName());
}
UAsync_SetTimer::~UAsync_SetTimer()
{
	UE_LOG(LogAsyncAction, Log, TEXT("UAsync_SetTimer::~UAsync_SetTimer(): Async_SetTimer object is being deleted."));
}

UAsync_SetTimer* UAsync_SetTimer::SetTimer(const UObject* WorldContextObject, float Time, bool bLooping, float InitialStartDelay /*= 0.f*/, float InitialStartDelayVariance /*= 0.f*/)
{
	if (!WorldContextObject)
	{
		FFrame::KismetExecutionMessage(TEXT("Invalid WorldContextObject. Cannot execute Set Timer."), ELogVerbosity::Error);
		return nullptr;
	}

	/** Based on UKismetSystemLibrary::K2_SetTimer() */
	InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
	if (Time <= 0.f || ((Time + InitialStartDelay) - InitialStartDelayVariance) < 0.f)
	{
		FFrame::KismetExecutionMessage(TEXT("SetTimer passed a negative or zero time.  The associated timer may fail to fire!  If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), ELogVerbosity::Warning);
		return nullptr;
	}

	UAsync_SetTimer* AsyncNode = NewObject<UAsync_SetTimer>();
	AsyncNode->WorldContextObject = WorldContextObject;

	FTimerDynamicDelegate Delegate;

	Delegate.BindUFunction(AsyncNode, FName("CompletedEvent"));
	AsyncNode->TimerHandle = AsyncNode->SetTimerDelegate(Delegate, Time, bLooping, InitialStartDelay, InitialStartDelayVariance);

	//AsyncNode->AddToRoot();
	//AsyncNode->RemoveFromRoot();
	//  Call to globally register this object with a game instance, it will not be destroyed until SetReadyToDestroy is called
	AsyncNode->RegisterWithGameInstance((UObject*)WorldContextObject);
	TimerHandleInstMap.Emplace(AsyncNode->TimerHandle.ToString(), AsyncNode);

	return AsyncNode;


}

void UAsync_SetTimer::ClearAndInvalidateTimerHandle(UObject* WorldContextObject, UPARAM(ref)FTimerHandle& Handle)
{
	if (Handle.IsValid())
	{
		// Get UAsync_SetTimer object
		UAsync_SetTimer* Node = *TimerHandleInstMap.Find(Handle.ToString());
		if (Node)
		{
			// ready to destroy this UAsync_SetTimer object
			Node->SetReadyToDestroy();
			//remove from map
			TimerHandleInstMap.Remove(Handle.ToString());
		}
		//Clears a set timer.
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(WorldContextObject, Handle);
	}
}



FTimerHandle UAsync_SetTimer::SetTimerDelegate(FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay /*= 0.f*/, float InitialStartDelayVariance /*= 0.f*/)
{
	FTimerHandle Handle;

	if (Delegate.IsBound())
	{
		const UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		if (World)
		{
			InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
			if (Time <= 0.f || ((Time + InitialStartDelay) - InitialStartDelayVariance) < 0.f)
			{
				FFrame::KismetExecutionMessage(TEXT("SetTimer passed a negative or zero time.  The associated timer may fail to fire!  If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), ELogVerbosity::Warning);
			}

			FTimerManager& TimerManager = World->GetTimerManager();
			Handle = TimerManager.K2_FindDynamicTimerHandle(Delegate);
			TimerManager.SetTimer(Handle, Delegate, Time, bLooping, (Time + InitialStartDelay));
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Warning,
			TEXT("SetTimer passed a bad function (%s) or object (%s)"),
			*Delegate.GetFunctionName().ToString(), *GetNameSafe(Delegate.GetUObject()));
	}

	return Handle;


}

void UAsync_SetTimer::Activate()
{
	if (!WorldContextObject)
	{
		FFrame::KismetExecutionMessage(TEXT("Invalid WorldContextObject. Cannot execute Set Timer."), ELogVerbosity::Error);
		return;
	}

	// call Then delegate binding event
	Then.Broadcast(TimerHandle);
}

void UAsync_SetTimer::CompletedEvent()
{
	Completed.Broadcast(TimerHandle);
}