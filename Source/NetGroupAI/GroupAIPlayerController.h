// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetGroupAIGameMode.h"
#include "PlayerCharacter.h"
#include "NetGroupAICharacter.h"
#include "ObserverPlayer.h"
#include "GroupAIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NETGROUPAI_API AGroupAIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay();

	//UFUNCTION(Server,Reliable)
	//void CreatePlayerCharacter();

	//服务器上运行，生成玩家角色
	UFUNCTION(BlueprintCallable)
	void CreatePlayerCharacter_Implementation();

//	void print();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Initialize)
		TSubclassOf<APlayerCharacter> BasePlayerType;






	
};
