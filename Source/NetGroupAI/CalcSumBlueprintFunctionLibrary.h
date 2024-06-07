// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyStruct.h"
#include "CalcSumBlueprintFunctionLibrary.generated.h"


//从栈上取出泛型数组参数，并正确的移动栈指针
//通过这个宏 ，可以得到两个局部变量
//void * ArrayAddr 数组的起始内存地址
//UArrayProperty *ArrayProperty :数组的反射信息，ArrayProperty->Inner 数组成员对应的类型
#define P_GET_GENERIC_ARRAY(ArrayAddr,ArrayProperty) Stack.MostRecentProperty = nullptr;\
		Stack.StepCompiledIn<UArrayProperty>(NULL);\
		void * ArrayAddr = Stack.MostRecentPropertyAddress;\
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);\
		if (!ArrayProperty) {	Stack.bArrayContextFailed = true;	return; }


/*
	实现蓝图功能节点的几种方式：
	1.UFUNCTION 宏修饰，并在宏里面添加blueprintCallable标识符，就可以
		自动完成蓝图编辑节点、蓝图节点执行调用的整个过程，但由于c++和蓝图
		都是静态类型编程语言，这种形式编写的蓝图节点，所有的输入、输出参数
		的类型都必须是固定的，这样引擎才能自动处理蓝图虚拟机的栈

	2.实现class UK2Node的派生类，这是最强大的方式，是对蓝图节点最深入的开发
		可以动态的添加\删除蓝图节点的阵脚就只能使用这种方式，例如“format Text"
		节点，可以根据输入字符串中的{index} 来动态增加输入节点，输入节点的类型
		也是动态的，这个通过class UK2Node_FormatText 这个类实现

	3.在UFUNCTION中使用CustomThunk标识符，告诉UHT不要生成默认的蓝图包装函数，而是由我们手动实现
		这种方式需要控制蓝图虚拟机的栈，但是不用处理蓝图编辑去UI部分
		（这种方式结合meta标识符，可以实现参数类型的"通配符"，就可以实现模板函数，也就是输入、输出参数
			可以处理多种数据类型，类似c++的泛型。)
			meta标识符：
		(1.ArrayParm = "parameter1,parameter2,.." :说明BlueprintCallable函数应使用一个Call Array Function节点，且列出的参数应被视为通配符数组属性）
		（2.ArrayTypeDepentParams = "Param":使用ArrayParam时，此说明符指定一个参数，其将确定ArrayParm列表中所有参数的类型
		（3.CustomStructureParams = "Param1,params2,..":列出的参数都会被视为通配符）

	4.蓝图的"宏" -Macros,也可以实现自己的节点
*/


USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct
{
	GENERATED_USTRUCT_BODY()
};

/**
 * 
 */
UCLASS()
class NETGROUPAI_API UCalcSumBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		static int Sum(int a, int b);

	//实践1：接受任意UStruct类型参数
	//CustomThunk使用自定义的thunk包裹函数
	//声明了一个UFuncion,其参数类型是FMyStruct，这是一个占位符
	//UFUNCTION宏里面指定CustomThunk 和CustomStructureParam-》列出的参数作为通配符
	//实现一个execShowStructFields函数，主要处理蓝图的Stack，从中取出需要的参数，然后队c++调用
	UFUNCTION(BlueprintCallable,CustomThunk,Category = "MyDemo",meta = (CustomStructureParam = "CustomStruct"))
		static void ShowStructFields(const FDummyStruct& CustomStruct);
	
	//具体功能实现
	//打印类名和类型名
	static void Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty);

	DECLARE_FUNCTION(execShowStructFields)
	{
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		//实际调用UObject::execArrayGetByRef()
		//更新”FFrame::PropertyChainForCompiledIn”这个成员变量
		//使用更新后的“FFrame::PropertyChainForCompiledIn”值，更新了”FFrame::MostRecentPropertyAddress”成员变量。
		//即获取了蓝图虚拟机的栈顶指针
		//Stack.MostRecentPropertyAddress和Stack.MostRecentProperty这两个变量，就是当前参数值的内存地址和反射信息
		Stack.StepCompiledIn<FStructProperty>(NULL);

		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = Cast<FStructProperty>(Stack.MostRecentProperty);


		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_ShowStructFields(StructAddr, StructProperty);
		P_NATIVE_END;

	}


	//实践二：对数组中的struct的数值型成员求平均

	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "Array Numeric Property Average", ArrayParm = "TargetArray", ArrayTypeDependentParams = "TargetArray"), Category = "MyDemo")
		static float Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName PropertyName);

	static float GenericArray_NumericPropertyAverage(const void* TargetArray, const UArrayProperty* ArrayProperty, FName ArrayPropertyName);

	DECLARE_FUNCTION(execArray_NumericPropertyAverage)
	{
		//get targetArray
		//数组的起始地址，数组的反射信息
		P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty);

		//get propertyName
		P_GET_PROPERTY(FNameProperty, PropertyName);

		P_FINISH;

		P_NATIVE_BEGIN;
		*(float*)RESULT_PARAM = GenericArray_NumericPropertyAverage(ArrayAddr, ArrayProperty, PropertyName);
		P_NATIVE_END;

	}

};
