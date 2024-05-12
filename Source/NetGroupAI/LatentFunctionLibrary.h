// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include"Kismet/KismetSystemLibrary.h"
#include "LatentFunctionLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)//实现laten操作的多节点输出
enum class DELAY_EXEC :uint8
{
	HalfExec,
	CompletedExec
};


UCLASS()//蓝图函数库功能，实现类似蓝图Delay节点的latent潜在操作
class NETGROUPAI_API ULatentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "exec", Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
		static void TwiceDelay(const UObject* WorldContextObject,  //对应的蓝图中调用此节点的actor(自赋值)
												DELAY_EXEC&exec, 
												float Duration, 
												struct FLatentActionInfo LatentInfo);
	//有两次输出，因此使用元数据说明符ExpandEnumAsExecs，(枚举类型作为多个输出节点)
	//LatentInfo = "LatentInfo"，表明该函数是lantenAction,LatentInfo是对应函数参数
	//HidePin 和 DefaultToSelf 联合使用，意味隐藏参数，以及自赋值
};
