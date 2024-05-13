// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Engine/EngineTypes.h"
#include "Engine.h"
#include "Async_SetTimer.generated.h"


//用于实现一个可同时处理N个对象的异步调用功能的SetTimer
//参考https://zhuanlan.zhihu.com/p/107021667：将异步操作封装为蓝图节点
DECLARE_LOG_CATEGORY_EXTERN(LogAsyncAction,Log,All);

/**
 * 
 */
UCLASS(meta = (HideThen = true))//隐藏默认的then执行流(使用我们自己的then)
class NETGROUPAI_API UAsync_SetTimer : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

    //为finishd delay 声明一个委托,Delegate的参数将出现在异步蓝图节点
	//FTimerHandleDelegate 的成员变量都是输出引脚，参数是对应输出引脚附带参数
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerHandleDelegate, FTimerHandle, TimerHandle);

	

public:

	UAsync_SetTimer();
	~UAsync_SetTimer();


	//Set a timer to execute Completed delegate.
	//Setting an existing timer will reset that timer with updated parameters.
	/*声明异步节点：此处声明了该蓝图节点，BlueprintInternalUseOnly = "true"，
	 *隐藏自动生成的非异步蓝图节点，没有这个说明符将在蓝图中看到二个同名节点。
	 *此函数的返回值必须是该派生类类型的指针，该返回值参数不会出现在蓝图节中，
	 *函数体主要用于输入参数检验、创建派生类对象以及赋值。
	  *GC: 将新建的UObject对象加入全局对象管理，避免自动回收*/
	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly = "true",WorldContext = "WorldContextObject",DisplayName="Set Timer",ScriptName="SetTimerDelegate",AdvancedDisplay= "InitialStartDelay, InitialStartDelayVariance"),Category= "Utilities|Time")
		static UAsync_SetTimer* SetTimer(const UObject* WorldContextObject, 
										float Time,bool bLooping,
										float InitialStartDelay = 0.f,
										float InitialStartDelayVariance = 0.f);

	//Clears a set timer and set ready to destroy UAsync_SetTimer object
	//GC:需要进行对象管理，操作主动回收，避免内存泄露
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear and Invalidate Timer by Handle(Async)", ScriptName = "ClearAndInvalidateTimerHandle", WorldContext = "WorldContextObject"), Category = "Utilities|Time")
		static void ClearAndInvalidateTimerHandle(UObject* WorldContextObject, 
												  UPARAM(ref) FTimerHandle& Handle);

	/** Generate Exec Outpin, named Then */
	//声明exec pin: 输出引脚
	UPROPERTY(BlueprintAssignable)
		FTimerHandleDelegate Then;

	/** Generate Exec Outpin, named Completed */
	UPROPERTY(BlueprintAssignable)
		FTimerHandleDelegate Completed;

private:
	//Activate()函数：基类UBlueprintAsyncActionBase的接口，在函数体内部触发委托绑定的事件。
	virtual void Activate()override;

	/** UFunction for Delegate.BindUFunction; */
	UFUNCTION()
		void CompletedEvent();

	/** Based on UKismetSystemLibrary::K2_SetTimerDelegate() */
	FTimerHandle SetTimerDelegate(FTimerDynamicDelegate Delegate, 
								  float Time, bool bLooping, 
								  float InitialStartDelay = 0.f, 
								  float InitialStartDelayVariance = 0.f);
private:
	const UObject* WorldContextObject;
	FTimerHandle TimerHandle;
	



};
