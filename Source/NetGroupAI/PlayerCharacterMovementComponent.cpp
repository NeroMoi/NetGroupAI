// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include"Engine/Engine.h"

void UPlayerCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//获取并清理：：Tick中设置的移动向量
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{

		//调用服务器更新移动组件信息
		UpdateMove(DesiredMovementThisFrame);

		//是否是本地客户端(使用服务器同步后的移动组件信息，更新角色移动)
		//PawnMovement未做同步，使用同步CharacterMovement


		if (this->PawnOwner->IsLocallyControlled())
		{
			

			//方式一：使用服务器同步的Location信息，设置本地actor的位置信息
		/*	auto Player = Cast<APlayerCharacter>(GetPawnOwner());
			Player->SetActorLocation(Player->PlayerLocation);
			auto location = this->UpdatedComponent->GetComponentLocation();
			*/
		
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("client:x:%f y:%f z:%f"), location.X, location.Y, location.Z));

			
			//方式二：使用客户端的输入信息，使用移动组件设置本次的位置信息
			FHitResult Hit;
			SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);


			//如果发生碰撞，尝试滑过去
			if (Hit.IsValidBlockingHit())
			{
				SlideAlongSurface(DesiredMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
			}


			
		}

	}






}

void UPlayerCharacterMovementComponent::UpdateMove_Implementation(FVector DesireMovementThisFrame)
{


	FHitResult Hit;
	SafeMoveUpdatedComponent(DesireMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);


	//如果发生碰撞，尝试滑过去
	if (Hit.IsValidBlockingHit())
	{
		SlideAlongSurface(DesireMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
	}

/*	auto location = this->UpdatedComponent->GetComponentLocation();
	auto Player = Cast<APlayerCharacter>(GetPawnOwner());
	Player->PlayerLocation = location;
	*/

//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("server:x:%f y:%f z:%f"), location.X, location.Y, location.Z));

}
