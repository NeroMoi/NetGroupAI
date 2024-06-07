// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupAIPlayerController.h"
#include "ObserverPlayer.h"
#include "ObserverPlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include"Engine/Engine.h"

void AGroupAIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
	//本地生成HUD->中心点绘制准心
	InitializeHUD();
	//服务器生成角色
	CreatePlayerCharacter_Implementation();
	//开启鼠标overlap事件
	//用于执行，鼠标指向actor，actor做出响应
	this->bEnableMouseOverEvents = true;

	
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
			//	FString BaseName;
			//	BasePlayerType->GetName(BaseName);
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, BaseName);
				//服务器上的客户端对应的每一个控制器对应生成一个玩家角色
				if (!this->IsLocalController())
				{
					//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetWorld()->GetMapName());
					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(BasePlayerType,GM->playerStart->GenerateStartLocation(), FRotator::ZeroRotator);
			
					if (PlayertInstance)
					{
						// 搭配蓝图部分测试，实例化的属性，在蓝图实例化对象时，属性是拷贝的蓝图CDO，因此在创建指针成员时尤其需要注意
					/*	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, PlayertInstance->GetFName().ToString());
						auto temp = Cast<APlayerCharacter>(PlayertInstance->TestInstanceObj->Myowner);
						if (temp)
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, temp->GetFName().ToString());
						*/
						
						
						PlayertInstance->SetOwner(this);
						this->Possess(PlayertInstance);
						PlayertInstance->UniqueID = this->GetUniqueID();
					}
				}
				else//服务器玩家角色
				{

					/*APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(BasePlayerType, GM->playerStart->GenerateStartLocation(), FRotator::ZeroRotator);

					if (PlayertInstance)
					{
						
						PlayertInstance->SetOwner(this);
						this->Possess(PlayertInstance);
						PlayertInstance->UniqueID = this->GetUniqueID();
						PlayertInstance->SetActorLocation(FVector(80.0f, 80.0f, 80.0f));
					}*/

					APlayerCharacter* ServerPlayer = Cast<APlayerCharacter>(GetPawn());
					if (ServerPlayer)
					{
						ServerPlayer->UniqueID = this->GetUniqueID();
						ServerPlayer->SetActorLocation(FVector(80.0f, 80.0f, 80.0f));
					}

					//销毁gameMode默认生成
				/*	if (GetPawn())
					{
						GetPawn()->Destroy();
					}

					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(BasePlayerType,FVector(80, 80, 80), FRotator::ZeroRotator);
					if (PlayertInstance)
					{
						PlayertInstance->SetOwner(this);
						this->Possess(PlayertInstance);
					}*/
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
					/*	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, PlayertInstance->GetFName().ToString());
						auto temp = Cast<APlayerCharacter>(PlayertInstance->TestInstanceObj->Myowner);
						if (temp)
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, temp->GetFName().ToString());
						*/

						PlayertInstance->SetOwner(this);
					
						this->Possess(PlayertInstance);
						PlayertInstance->UniqueID = this->GetUniqueID();
					}
				}
				else
				{

					

					APlayerCharacter* ServerPlayer = Cast<APlayerCharacter>(GetPawn());
					if (ServerPlayer)
					{
						ServerPlayer->UniqueID = this->GetUniqueID();
						ServerPlayer->SetActorLocation(FVector(80.0f, 80.0f, 80.0f));
					}

					/*if (GetPawn())
					{
						GetPawn()->Destroy();
					}

					APlayerCharacter* PlayertInstance = GetWorld()->SpawnActor<APlayerCharacter>(FVector(80, 80, 80), FRotator::ZeroRotator);
					PlayertInstance->SetOwner(this);
					this->Possess(PlayertInstance);*/
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



}

void AGroupAIPlayerController::InitializeHUD()
{
	//只给本地控制器生成HUD，不需要同步

	if (IsLocalController())
	{
		//初始化HUD
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save HUDs into a map
		if (PlayerHUD)
			MyHUD = GetWorld()->SpawnActor<AObserverPlayerHUD>(PlayerHUD, SpawnInfo);

		if (this->GetHUD())
		{
			GetHUD()->bShowHUD = false;
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("have HUD"));
		}
		else
		{
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NO HUD"));
		}
	}


}
