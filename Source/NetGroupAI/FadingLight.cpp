// Fill out your copyright notice in the Description page of Project Settings.


#include "FadingLight.h"
#include"Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "HAL/Platform.h"
#include"Engine/Engine.h"

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

