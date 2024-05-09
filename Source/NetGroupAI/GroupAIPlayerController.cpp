// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupAIPlayerController.h"
#include "ObserverPlayer.h"
#include"Engine/Engine.h"

void AGroupAIPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CreatePlayerCharacter_Implementation();
}

void AGroupAIPlayerController::CreatePlayerCharacter_Implementation()
{
	
	//服务器上才能生成角色
	if (GetLocalRole() == ROLE_Authority)
	{
		
		
		ANetGroupAIGameMode* GM = GetWorld()->GetAuthGameMode<ANetGroupAIGameMode>();
		if (GM)
		{
			
			if (BasePlayerType)//蓝图派生
			{
				
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("使用蓝图"));
				FString BaseName;
				BasePlayerType->GetName(BaseName);
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, BaseName);
				//服务器上的客户端对应的每一个控制器对应生成一个玩家角色
				if (!this->IsLocalController())
				{
					//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetWorld()->GetMapName());
					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(BasePlayerType,GM->playerStart->GenerateStartLocation(), FRotator::ZeroRotator);
			
					if (PlayertInstance)
					{
						// 搭配蓝图部分测试，实例化的属性，在蓝图实例化对象时，属性是拷贝的蓝图CDO，因此在创建指针成员时尤其需要注意
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, PlayertInstance->GetFName().ToString());
						auto temp = Cast<APlayerCharacter>(PlayertInstance->TestInstanceObj->Myowner);
						if (temp)
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, temp->GetFName().ToString());
						
						
						PlayertInstance->SetOwner(this);
						this->Possess(PlayertInstance);
					}
				}
				else//服务器玩家角色
				{
					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(BasePlayerType,FVector(80, 80, 80), FRotator::ZeroRotator);
					PlayertInstance->SetOwner(this);
					this->Possess(PlayertInstance);
				}
			}
			else//原生c++代码
			{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,TEXT("使用c++"));
				//服务器上的每一个控制器对应生成一个玩家角色
				if (!this->IsLocalController())
				{

					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>( GM->playerStart->GenerateStartLocation(), FRotator::ZeroRotator);

					if (PlayertInstance)
					{

						  // 搭配蓝图部分测试，实例化的属性，在蓝图实例化对象时，属性是拷贝的蓝图CDO，因此在创建指针成员时尤其需要注意
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, PlayertInstance->GetFName().ToString());
						auto temp = Cast<APlayerCharacter>(PlayertInstance->TestInstanceObj->Myowner);
						if (temp)
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, temp->GetFName().ToString());
						

						PlayertInstance->SetOwner(this);
						this->Possess(PlayertInstance);
					}
				}
				else
				{
					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(FVector(80, 80, 80), FRotator::ZeroRotator);
					PlayertInstance->SetOwner(this);
					this->Possess(PlayertInstance);
				}
			}
		}
		
	
	/*	if (GM)
		{
			
			//服务器上的每一个控制器对应生成一个玩家角色
			if (!this->IsLocalController())
			{
		//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetWorld()->GetMapName());
				APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(GM->playerStart->GenerateStartLocation(),FRotator::ZeroRotator);

				if (PlayertInstance)
				{
			//		auto tempName = PlayertInstance->GetFName();
			//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, tempName.ToString());
			//		PlayertInstance->SetActorLocation(FVector(-50, 50, 80));
		//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("AcotValue:%f"), (PlayertInstance->GetActorLocation()).X));
					PlayertInstance->SetOwner(this);

					this->Possess(PlayertInstance);

			//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Client Character");
	
				}
			}
			else
			{
				APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(FVector(80, 80, 80), FRotator::ZeroRotator);
			//	PlayertInstance->SetActorLocation(FVector(-50, 0, 80));
				PlayertInstance->SetOwner(this);
				this->Possess(PlayertInstance);
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Server Character");
			}
		}*/

	}

	//UGameplayStatics::GetGameMode(UObject*WorldContextObject);

}
