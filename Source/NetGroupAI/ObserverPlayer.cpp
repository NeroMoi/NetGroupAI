// Fill out your copyright notice in the Description page of Project Settings.


#include "ObserverPlayer.h"

// Sets default values
AObserverPlayer::AObserverPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Shape = CreateDefaultSubobject<USphereComponent>(TEXT("Root component"));
	RootComponent = Shape;
	Shape->InitSphereRadius(20);
	Shape->SetCollisionProfileName(TEXT("Pawn"));

	SM_Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visualSphere"));
	SM_Shape->SetupAttachment(RootComponent);

	//硬编码资源路径
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		//让渲染组件的中心和球形组件中心对齐
		SM_Shape->SetStaticMesh(SphereVisualAsset.Object);
		SM_Shape->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		SM_Shape->SetWorldScale3D(FVector(0.4f));

	}

}

// Called when the game starts or when spawned
void AObserverPlayer::BeginPlay()
{
	Super::BeginPlay();

	SelfName = this->GetFName().ToString();
	//表示绕着X轴旋转180度，绕y轴旋转0度，Z轴旋转0度
	//基于四元数
	FQuat newRotation(FRotator(180, 0, 0));
	AddActorLocalRotation(newRotation);
}

// Called every frame
void AObserverPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(ActorRotationRate);

}

