// Fill out your copyright notice in the Description page of Project Settings.


#include "FadingLight.h"
#include"Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "HAL/Platform.h"
#include"Engine/Engine.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/MetaData.h"
#include "FadingLightInterface.h"
#include "UObject/Stack.h"

// Sets default values
AFadingLight::AFadingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComp"));
	LightTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("LightTimelineComp"));
	LightOverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("LightOverlapVolume"));

	RootComponent = LightOverlapVolume;
	PointLightComp->AttachToComponent(LightOverlapVolume, FAttachmentTransformRules::KeepRelativeTransform);
	
	BrightnessMultiplier = 20.0f;


}

void AFadingLight::TurnOnDirectionalLight()
{
	if(directionLight)
		directionLight->SetEnabled(true);

	directionLight = nullptr;
	//清理定时器
	GetWorldTimerManager().ClearTimer(UserTimerHandle);
}

void AFadingLight::UpdateLightBrightness_Implementation(float Brightnessout)
{
	PointLightComp->SetLightBrightness(Brightnessout*20.0f);

	
}

void AFadingLight::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		
		//关灯效果只对客户端自己起效果
		//这里监听服务器的拥有的角色他的remoteRole 是ROLE_AutonomousProxy，而不是专用服务器的ROLE_SimulatedProxy
		//因此如果使用监听服务器，应该使用IScontroller判断
		//player->GetLocalRole() == ROLE_AutonomousProxy
		if (player->IsLocallyControlled())//&&player->GetRemoteRole()==ROLE_SimulatedProxy)
		{
			
			//打印枚举值的名称
			UEnum* const ModeEnum = StaticEnum<ENetRole>();
			auto modeName = ModeEnum->GetDisplayNameTextByValue(static_cast<uint8>(player->GetRemoteRole())).ToString();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, modeName);


			TArray<AActor*>DirectionLightActor;
			if (GetWorld())
			{
				

				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), DirectionLightActor);
				for (auto Light : DirectionLightActor)
				{
					ADirectionalLight* tempActor = Cast<ADirectionalLight>(Light);
					if (tempActor)
					{
						
						GetWorld()->GetTimerManager().ClearTimer(UserTimerHandle);
						tempActor->SetEnabled(false);
						


					}


				}

			}
			//使用时间轴
			LightTimeLineComp->PlayFromStart();
		}
	}

	


}

void AFadingLight::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
	
		if (player->IsLocallyControlled())
		{

			TArray<AActor*>DirectionLightActor;
			UWorld* world = GetWorld();
			if (world)
			{


				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), DirectionLightActor);
				for (auto Light : DirectionLightActor)
				{
					ADirectionalLight* tempActor = Cast<ADirectionalLight>(Light);
					if (tempActor)
					{
						directionLight = tempActor;

						//定时器这里存在一个逻辑错误
						//1.overlap结束事件，一定时间后才会打开directionalLight
						//2. overlaoBegin时间，不会检测定时器的事件，导致可能关了又开
						world->GetTimerManager().SetTimer(UserTimerHandle, this,&AFadingLight::TurnOnDirectionalLight, 10.0f, false);
						

					}


				}

			}

			LightTimeLineComp->ReverseFromEnd();
			
		}
	}

}
int32 AFadingLight::printInfo(float param)
{
	return param;
}

int32 AFadingLight::InvokeFunction(UObject* obj, FName functionName, float Param1)
{

	//定义一个结构体包装参数和返回值，类似gen.cpp
	//先参数后返回值的顺序
	struct MyClass_Func_Params
	{
		float param1;
		int32 ReturnValue;

	};

	UFunction* func = obj->FindFunctionChecked(functionName);

	MyClass_Func_Params params;

	params.param1 = Param1;

	//调用形式1
	obj->ProcessEvent(func, &params);

	//调用形式2
	FFrame frame(nullptr, func, &params, nullptr, func->ChildProperties);
	obj->CallFunction(frame, &params + func->ReturnValueOffset, func);

	//调用形式3
	FFrame frame1(nullptr, func,&params, NULL, func->ChildProperties);
	func->Invoke(obj, frame1, &params + func->ReturnValueOffset);

	//调用1，2，3是等价的，在没有obj的情况下调用static函数，可以调用3 的方式
	//蓝图里面的事件和函数最终都会编译生成UFunction对象，所以用此方式可以直接
	//调用蓝图里的成员函数和自定义事件




	return params.ReturnValue;

}

void AFadingLight::TestReflection()
{
	
	//获取类型对象
	{
		TArray<UObject*> result;

		//获取所有的class和Interface类型
		GetObjectsOfClass(UClass::StaticClass(), result);
		//获取所有的enum类型
		GetObjectsOfClass(UEnum::StaticClass(), result);
		//获取所有的struct类型
		GetObjectsOfClass(UScriptStruct::StaticClass(), result);

	}
	//根据一个名字查找某个类型对象

	{
		//获得表示FadingLight类型的UClass对象
		UClass* classobj = FindObject<UClass>(ANY_PACKAGE, TEXT("FadingLight"));


	}


	{

		//获取到类型对象后，遍历字段TFieldIterator<T>

		UClass* classObj = AFadingLight::StaticClass();

		//遍历属性
		for (TFieldIterator<UProperty>i(classObj); i; ++i)
		{
			UProperty* prop = *i;

		}
		//遍历函数
		for (TFieldIterator<UFunction>i(classObj); i; ++i)
		{

			UFunction* func = *i;

			for (TFieldIterator<UProperty>j(func); j; ++j)
			{
				UProperty* param = *j;

				//这是返回值
				if (param->PropertyFlags & CPF_ReturnParm)
				{

				}

			}

		}
	}

		 //遍历接口

		 {
			 UClass* classObj = AFadingLight::StaticClass();

			 for (const FImplementedInterface& ii : classObj->Interfaces)
			 {
				 UClass* interfaceClass = ii.Class;
			 }


		 }

		 //遍历枚举

		 {
			 const UEnum* enumClass = FindObject<UEnum>(ANY_PACKAGE,TEXT("FadingLightType"));

			 for (int i = 0; i < enumClass->NumEnums(); ++i)
			 {
				 FName name = enumClass->GetNameByIndex(i);
				 int value = enumClass->GetValueByIndex(i);
			 }

		 }

		 //遍历元数据
		 {
			#if WITH_METADATA

			 const UObject* obj = AFadingLight::StaticClass(); //可以是任何对象，但一般是UField才有值

			 UMetaData* metaData = obj->GetOutermost()->GetMetaData();

			 TMap<FName, FString>* keyValues = metaData->GetMapForObject(obj);

			 if (keyValues != nullptr && keyValues->Num() > 0)
			 {
				 for (const auto& i : *keyValues)
				 {
					 FName Key = i.Key;
					 FString value = i.Value;

				 }
			 }

			#endif

		 }

		 //根据名字精确查找属性

		 {
			 UProperty* obj = AFadingLight::StaticClass()->FindPropertyByName(TEXT("BrightnessMultiplier"));

		 }

		 //精确名字查找函数
		 {
			 UFunction* obj = AFadingLight::StaticClass()->FindFunctionByName(TEXT("UpdateLightBrightness"), /*EIncludeSuperFlag::Type*/ EIncludeSuperFlag::Type::IncludeSuper);


		 }

		 //查看继承关系

		 {
			 UStruct* obj = AFadingLight::StaticClass();
			 TArray<FString> classNames;

			 classNames.Add(obj->GetName());
			 UStruct* superClass = obj->GetSuperStruct();
			 while (superClass)
			 {
				 classNames.Add(superClass->GetName());
				 superClass = superClass->GetSuperStruct();
			 }

			 //会输出myclass->Uobject
			 FString str = FString::Join(classNames, TEXT("->"));


		 }

		 //获取一个类下面的子类
		 {
			 const UClass* classObj = AFadingLight::StaticClass();
			 TArray<UClass*> result;

			 GetDerivedClasses(classObj, result, false);

		 }
		 //获取实现了某个接口的所有子类

		 {
			 //存储所有表示类型的对象
			 TArray<UObject*> result;

			 //所有的表示类型的UClass对象，本身也是UObject对象
			 GetObjectsOfClass(UClass::StaticClass(), result);

			 TArray<UClass*> classes;

			 //查找的接口对应的UClass
			 UClass* InterfaceObj = FindObject<UClass>(ANY_PACKAGE, TEXT("IFadingLightInterface"));

			 for (UObject* obj : result)
			 {
				 UClass* classObj = Cast<UClass>(obj);

				 

				 //判断实现了某个接口
				 if (classObj->ImplementsInterface(InterfaceObj))
				 {
					 classes.Add(classObj);
				 }


			 }


		 }

		 {
			 //获取设置属性值：获得了UProperty就可以获得其值



			 FProperty* obj = AFadingLight::StaticClass()->FindPropertyByName(TEXT("BrightnessMultiplier"));

			 //方式一
			 FFloatProperty* floatProperty = Cast<FFloatProperty>(obj);
			 float BrightnessMultiplierValue1 = floatProperty->GetPropertyValue_InContainer(this, 0);

			//方式二
			 //获取对象或结构里的属性值地址，需要自己转换成具体类型
			 void* PropertyValuePtr = obj->ContainerPtrToValuePtr<void*>(this);

			 float BrightnessMultiplierValue = *((float*)(PropertyValuePtr));


			 //包含对象引用的属性可以获得对象
			 
			 FProperty* oProperty = AFadingLight::StaticClass()->FindPropertyByName(TEXT("PointLightComp"));
			 FObjectProperty* objProperty = Cast<FObjectProperty>(oProperty);
			 UPointLightComponent *instance =  Cast<UPointLightComponent> (objProperty->GetObjectPropertyValue_InContainer(this));
			  
			  


		 }

		 {
			 //填充属性值
			 //通过字符串导入或导出属性值
			 //ExportTextItem和ImportText实际上就算我们在编辑器里的Detail面板上选择一个属性进行ctrl+c 和ctrl +v的调用方法
			 //UE4把他们序列化成字符串传递的
			 FProperty* PropertyObj = AFadingLight::StaticClass()->FindPropertyByName(TEXT("BrightnessMultiplier"));

			 void* PropertyValuePtr = PropertyObj->ContainerPtrToValuePtr<void*>(this);

			 //导出
			 FString outPropertyValueString;

			 PropertyObj->ExportTextItem(outPropertyValueString, PropertyValuePtr, nullptr,(UObject*)this,PPF_None);

			 //导入

			 FString ValueStr(TEXT("666.6"));
			 PropertyObj->ImportText(*ValueStr, PropertyValuePtr, PPF_None, this);




		 }

		 {
			 //反射调用函数
			 

			 int result = InvokeFunction(this, TEXT("printInfo"), 123.f);

		 }

		 


	
	





}

// Called when the game starts or when spawned
void AFadingLight::BeginPlay()
{
	Super::BeginPlay();

	updateBrightnessTrack.BindDynamic(this, &AFadingLight::UpdateLightBrightness);
	UpdateColorTrack.BindDynamic(this, &AFadingLight::UpdateLightColor);

	//如果有浮点曲线，将他的图表与我们的更新函数绑定
	if (pointLightFloatCurve)
	{
		LightTimeLineComp->AddInterpFloat(pointLightFloatCurve, updateBrightnessTrack);

	}

	if (pointLightColorCurve)
	{
		LightTimeLineComp->AddInterpLinearColor(pointLightColorCurve, UpdateColorTrack);

	}

	//初始化碰撞盒子的overlap事件
	LightOverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &AFadingLight::OnOverlapBegin);
	LightOverlapVolume->OnComponentEndOverlap.AddDynamic(this, &AFadingLight::OnOverlapEnd);
}

// Called every frame
void AFadingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

