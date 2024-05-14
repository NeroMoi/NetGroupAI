// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine/World.h"
#include"Engine/Engine.h"
#include "Engine/EngineTypes.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphereComponent;
	sphereComponent->InitSphereRadius(20.0f);
	sphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	

	SM_sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visualSphere"));
	SM_sphere->SetupAttachment(RootComponent);
//	SM_sphere->SetSimulatePhysics(true);
	
	//硬编码资源路径
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		//让渲染组件的中心和球形组件中心对齐
		SM_sphere->SetStaticMesh(SphereVisualAsset.Object);
		SM_sphere->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		SM_sphere->SetWorldScale3D(FVector(0.4f));

	}

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraSpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength = 400.0f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	
	PlayerCamera->SetupAttachment(CameraSpringArm,USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;
	

	//控制默认玩家->server/client模式，因此controller对应控制生成的player
//	AutoPossessPlayer = EAutoReceiveInput::Player0;


	playerMovement = CreateDefaultSubobject<UPlayerCharacterMovementComponent>(TEXT("CustomMovementComponent"));
	playerMovement->UpdatedComponent = RootComponent;
	playerMovement->SetIsReplicated(true);

	//默认未隐藏鼠标
	bHideMouse = false;

	//指向的是原生类默认对象
	/*TestInstanceObj = CreateDefaultSubobject<UTestInstanceObject>(TEXT("Test object"));
	TestInstanceObj->testValue = 55.0f; 
	TestInstanceObj->Myowner = this;*/


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//处理角色俯仰和偏航，以及视角放大缩小
	if (IsLocallyControlled())
	{
		//根据帧率更新平滑旋转
		AdjustRotation(DeltaTime);
	}

	
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//放大缩小
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed,this,&APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released,this,&APlayerCharacter::ZoomOut);

	//显示鼠标后，才能使用鼠标控制移动
	PlayerInputComponent->BindAction("MapDisplayMouse", IE_Pressed, this, &APlayerCharacter::MapDispalyMouseIn);
	PlayerInputComponent->BindAction("MapDisplayMouse", IE_Released, this, &APlayerCharacter::MapDispalyMouseOut);

	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &APlayerCharacter::TeleportIn);
	PlayerInputComponent->BindAction("Teleport", IE_Released, this, &APlayerCharacter::TeleportOut);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("Up", this, &APlayerCharacter::Up);

}

UPawnMovementComponent* APlayerCharacter::GetMovementComponent() const
{
	return playerMovement;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerCharacter, PlayerLocation, COND_OwnerOnly);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	
	if (playerMovement && (playerMovement->UpdatedComponent == RootComponent))
	{
		/* //如果使用摄像机的旋转，则是xyz三维空间的向量
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		*/

		//使用actor自身朝向作为输入向量(限定在了xy坐标面)
		playerMovement->AddInputVector(GetActorForwardVector() * AxisValue);

	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "forward");
	}

	
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	
	if (playerMovement && (playerMovement->UpdatedComponent == RootComponent))
	{
		/* //不跟随摄像机的偏航旋转(yaw)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		playerMovement->AddInputVector(GetActorRightVector() * AxisValue);

		*/

		//使用actor自身朝向作为输入向量(限定在了xy坐标面)
		playerMovement->AddInputVector(GetActorRightVector() * AxisValue);
		
	}
}

void APlayerCharacter::Turn(float AxisValue)
{
	//处理逻辑放在了tick里面
	cameraInput.X = AxisValue;
	

	/*FRotator newRotation = GetActorRotation();
	newRotation.Yaw += AxisValue;
	SetActorRotation(newRotation);*/

}

void APlayerCharacter::Up(float AxisValue)
{
	cameraInput.Y = AxisValue;


	//处理逻辑放在了tick里面
/*	FRotator newRotation = GetActorRotation();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("pitch:%f"), newRotation.Pitch));

	newRotation.Pitch += AxisValue;
		
	if (newRotation.Pitch < -45.f)
		newRotation.Pitch = -45.f;

	if (newRotation.Pitch > 45.f)
		newRotation.Pitch = 45.f;

	SetActorRotation(newRotation);
	*/
	
}

void APlayerCharacter::ZoomIn()
{
	bZoomingIn = true;
	UserHealth -= 5;
	UserEnergy -= 10;

	BroadcastCurrentEnergy();
	BroadcastCurrentHealth();
	
}

void APlayerCharacter::ZoomOut()
{
	bZoomingIn = false;


}

void APlayerCharacter::MapDispalyMouseIn()
{
	bHideMouse= !bHideMouse;
	
}

void APlayerCharacter::MapDispalyMouseOut()
{
	if (bHideMouse == false)//显示鼠标->则不控制玩家转向
	{
		auto pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			
			if(!pc->bShowMouseCursor)
			pc->bShowMouseCursor = true;

			//设置鼠标不控制玩家旋转
			pc->SetIgnoreLookInput(true);
		}
	}
	else//隐藏鼠标
	{
		auto pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			//显示鼠标
			pc->bShowMouseCursor = false;
			//设置鼠标控制玩家旋转
			pc->ResetIgnoreLookInput();
		}
	}
}

void APlayerCharacter::TeleportIn()
{
	bTeleport = true;
	
}

void APlayerCharacter::TeleportOut()
{
	if (bHideMouse== false && bTeleport == true)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			auto pc =Cast<APlayerController>(GetController());
			if (pc)
			{
				ULocalPlayer* LocalPlayer = pc->GetLocalPlayer();
				if (LocalPlayer && LocalPlayer->ViewportClient)
				{
					//屏幕空间的鼠标坐标
					FVector2D MousePosition;

					if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
					{
						
					
						FVector MouseWorldLocation;
						FVector MouseWorldDirection;
						//获取屏幕空间到世界空间的坐标转换与方向向量
						pc->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, MouseWorldLocation, MouseWorldDirection);

				//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Mouse:x%f, y%f"), MousePosition.X, MousePosition.Y));
				//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Mouse:x%f, y%f,z%f"), MouseWorldLocation.X, MouseWorldLocation.Y, MouseWorldLocation.Z));
				//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Direction:x%f, y%f,z%f"), MouseWorldDirection.X, MouseWorldDirection.Y, MouseWorldDirection.Z));

						FHitResult HitResult;
						
					
						//这条射线是WorldStatic,所有与wolrdStatic类型block的对象都会挡住射线
						world->LineTraceSingleByChannel(HitResult, MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 1000, ECC_WorldStatic);

						if (HitResult.bBlockingHit)//如果检测到了碰撞
						{ 
						//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("destination:x%f, y%f,z%f"), HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z));
							//使用RPC通知服务器更新信息
							
							//actor的一半高度->避免陷入地面
							
							FVector Destination = FVector(0.0f, 0.0f, SM_sphere->GetCollisionShape().Box.HalfExtentY);
						//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("height:%f"), SM_sphere->GetCollisionShape().Box.HalfExtentY));
							Teleport(HitResult.Location+ Destination);
							SetActorLocation(HitResult.Location+ Destination);

						}
						else
						{
							

							Teleport(MouseWorldLocation + MouseWorldDirection * 1000);
							SetActorLocation(MouseWorldLocation + MouseWorldDirection * 1000);
						}


						
					}
				}

			}
		}

		
	}


	bTeleport = false;
}

void APlayerCharacter::Teleport_Implementation(FVector newLocation)
{

	SetActorLocation(newLocation);

}

void APlayerCharacter::AdjustRotation(float DeltaTime)
{

	//按下则放大
	if (bZoomingIn)
	{
		zoomFactor += DeltaTime/0.5;
	}
	else
	{
		zoomFactor -= DeltaTime/0.25;
	}

	zoomFactor = FMath::Clamp(zoomFactor, 0.0f, 1.0f);

	PlayerCamera->SetFieldOfView(FMath::Lerp<float>(90.0f, 60.0f, zoomFactor));
	CameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, zoomFactor);

	//控制角色偏航角
	{
		FRotator newRotator = GetActorRotation();
		newRotator.Yaw += cameraInput.X;
		SetActorRotation(newRotator);
	}
	//控制摄像机
	{
		FRotator newRotation = CameraSpringArm->GetComponentRotation();

		newRotation.Pitch = FMath::Clamp(newRotation.Pitch + cameraInput.Y,-80.0f,-15.0f);
		CameraSpringArm->SetWorldRotation(newRotation);


	}


}

UUserWidget* APlayerCharacter::GetHUD()
{
	return CurrentWidget;
}

void APlayerCharacter::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

		//AddToViewport 交给蓝图去做
		/**if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}*/
	}
	

}

void APlayerCharacter::BroadcastCurrentHealth()
{
	UpdateHealth.Broadcast();
}

void APlayerCharacter::BroadcastCurrentEnergy()
{
	UpdateEnergy.Broadcast();
}

