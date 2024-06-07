// Fill out your copyright notice in the Description page of Project Settings.


#include "CalcSumBlueprintFunctionLibrary.h"
#include"Engine/Engine.h"


int UCalcSumBlueprintFunctionLibrary::Sum(int a, int b)
{
	return a + b;
}

void UCalcSumBlueprintFunctionLibrary::Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty)
{

	UScriptStruct* Struct = StructProperty->Struct;

	for (TFieldIterator<FProperty>iter(Struct); iter; ++iter)
	{
	
		/*	FScreenMessageString NewMessage;

		NewMessage.CurrentTimeDisplayed = 0.0f;
		NewMessage.Key = INDEX_NONE;
		NewMessage.DisplayColor = FColor::Blue;

		NewMessage.TimeToDisplay = 5;

		NewMessage.ScreenMessage = FString::Printf(TEXT("Property:[%s].[%s]"), *(Struct->GetName()), *(iter->GetName()));

		NewMessage.TextScale = FVector2D::UnitVector;

		GEngine->PriorityScreenMessages.Insert(NewMessage, 0);
		*/
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Property:[%s].[%s]"), *(Struct->GetName()), *(iter->GetName())));
			
	}


}

float UCalcSumBlueprintFunctionLibrary::Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName PropertyName)
{
	check(0);
	return 0.f;
}

float UCalcSumBlueprintFunctionLibrary::GenericArray_NumericPropertyAverage(const void* TargetArray, const UArrayProperty* ArrayProperty, FName PropertyName)
{
	
	UStructProperty* InnerProperty = Cast<UStructProperty>(ArrayProperty->Inner);

	if (!InnerProperty)
	{
		UE_LOG(LogTemp, Error, TEXT("Array inner property is not a ustruct!"));
		return 0.f;
	}

	UScriptStruct* Struct = InnerProperty->Struct;
	FString PropertyNameStr = PropertyName.ToString();
	UNumericProperty* NumProperty = nullptr;

	//获取指定名称的数组型成员变量反射信息
	for (TFieldIterator<UNumericProperty> iter(Struct); iter; ++iter) {
		if (Struct->PropertyNameToDisplayName(iter->GetFName()) == PropertyNameStr) {
			NumProperty = *iter;
			break;
		}
	}

	if (!NumProperty) {
		UE_LOG(LogTemp, Log, TEXT("Struct property NOT numeric = [%s]"),
			*(PropertyName.ToString())
		);
	}


	FScriptArrayHelper ArrayHelper(ArrayProperty, TargetArray);
	int Count = ArrayHelper.Num();
	float Sum = 0.f;

	if (Count <= 0)
		return 0.f;

	if (NumProperty->IsFloatingPoint())
		for (int i = 0; i < Count; i++) {
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetFloatingPointPropertyValue(ValuePtr);

		}
	else if (NumProperty->IsInteger()) {
		for (int i = 0; i < Count; i++) {
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetSignedIntPropertyValue(ValuePtr);
		}
	}
	// TODO: else if(enum类型)

	return Sum / Count;
}

