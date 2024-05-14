// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Templates/SharedPointer.h"
#include "TestInstanceObject.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotify_HealthUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotify_EnergyUpdate);

UCLASS(Blueprintable, BlueprintType)
class NETGROUPAI_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

		


public:

	
	// Sets default values for this pawn's properties
	APlayerCharacter();


	UPROPERTY(EditAnywhere, blueprintReadWrite)
		UStaticMeshComponent* SM_sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* sphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USpringArmComponent* CameraSpringArm;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UCameraComponent * PlayerCamera;


	//用来测试实例化Instance标签的属性在蓝图中的引用关系(是原生，还是蓝图)
	//没有Instance标签，则蓝图不会实例化对象，蓝图的类默认对象引用的也是原生c++ 创建的类默认对象
	//带有这个标签，则该属性实例化并和蓝图组成prefab
	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Instanced,Category="Test")
	//	UTestInstanceObject*TestInstanceObj;


	
	//FVector2D movementInput;
	//存储inputComponent的输入量
	FVector2D cameraInput;
	//镜头放大缩小因子
	float zoomFactor;
	bool bZoomingIn;

	//使用M显示鼠标
	UPROPERTY(BlueprintReadWrite)
	bool bHideMouse;

	//使用鼠标左键传送
	bool bTeleport;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UPlayerCharacterMovementComponent* playerMovement;

	//用于服务器同步到owner客户端的位置信息
	//逻辑执行于移动组件
	UPROPERTY(Replicated)
		FVector PlayerLocation;

	/** 用户的控件类*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** 用作用户的控件实例。*/
	UPROPERTY(BlueprintReadWrite)
		UUserWidget* CurrentWidget;


	//目前不使用rep_notify 来通知属性(单机)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly,meta=(ClampMin="0",ClampMax="100"))
		int UserHealth;

	//暴露给蓝图使用
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
		FNotify_HealthUpdate UpdateHealth;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly,meta= (ClampMin = "0", ClampMax = "100"))
		int UserEnergy;

	//暴露给蓝图使用
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
		FNotify_EnergyUpdate  UpdateEnergy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent()const override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void Turn(float AxisValue);
	
	void Up(float AxisValue);

	//放大缩小
	void ZoomIn();
	void ZoomOut();


	//显示鼠标
	void MapDispalyMouseIn();
	void MapDispalyMouseOut();

	//使用鼠标左键传送
	void TeleportIn();
	void TeleportOut();

	//通知服务器控制移动
	UFUNCTION(Server,Reliable)
	void Teleport(FVector newLocation);
	void Teleport_Implementation(FVector newLocation);


	void AdjustRotation(float DelteTime);
public:

	UFUNCTION(BlueprintCallable)
		UUserWidget* GetHUD();
		

	/** 移除当前菜单控件，并在指定类（如有）中新建控件。*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable)
		void BroadcastCurrentHealth();

	UFUNCTION(BlueprintCallable)
		void BroadcastCurrentEnergy();




};
