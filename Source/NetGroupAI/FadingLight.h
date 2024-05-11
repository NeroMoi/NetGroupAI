// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/DirectionalLight.h"
#include "FadingLight.generated.h"

UCLASS()
class NETGROUPAI_API AFadingLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFadingLight();

	//浮点曲线
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		UCurveFloat* pointLightFloatCurve;
	//线曲线
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UCurveLinearColor* pointLightColorCurve;

	//时间轴组件
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTimelineComponent* LightTimeLineComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UBoxComponent* LightOverlapVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPointLightComponent* PointLightComp;

	//控制点光源亮度的轨道,这是一个委托
	FOnTimelineFloat updateBrightnessTrack;

	//控制点光源颜色的轨道
	FOnTimelineLinearColor UpdateColorTrack;

	UPROPERTY(EditDefaultsOnly)
		float BrightnessMultiplier;

	FTimerHandle UserTimerHandle;

	//打开世界光照
	ADirectionalLight* directionLight;

	
	void TurnOnDirectionalLight();

	//使用原生实现，蓝图可覆写
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		void UpdateLightBrightness(float Brightnessout);
		void UpdateLightBrightness_Implementation(float Brightnessout);

		//在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void UpdateLightColor(FLinearColor ColorOutput);



	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
