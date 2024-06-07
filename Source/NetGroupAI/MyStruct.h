// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStruct.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable,BlueprintType)
struct  FMyStruct
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Names;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Value;



};
