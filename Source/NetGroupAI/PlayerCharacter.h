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
#include "PlayerCharacter.generated.h"
 
UCLASS()
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

	
	//FVector2D movementInput;
	//存储inputComponent的输入量
	FVector2D cameraInput;
	//镜头放大缩小因子
	float zoomFactor;
	bool bZoomingIn;

	//使用M显示鼠标
	bool bHideMouse;

	//使用鼠标左键传送
	bool bTeleport;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UPlayerCharacterMovementComponent* playerMovement;

	//用于服务器同步到owner客户端的位置信息
	//逻辑执行于移动组件
	UPROPERTY(Replicated)
		FVector PlayerLocation;


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
};
