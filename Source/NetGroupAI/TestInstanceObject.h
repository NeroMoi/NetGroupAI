// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestInstanceObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NETGROUPAI_API UTestInstanceObject : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		float testValue = 66.0;

	//用于测试，蓝图类对象的指针指向是否是CDO复制的问题
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	    UObject* Myowner;
	
};
