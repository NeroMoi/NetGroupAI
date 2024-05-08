// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerStartLocation.h"
#include "NetGroupAIGameMode.generated.h"

UCLASS(minimalapi)
class ANetGroupAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANetGroupAIGameMode();

	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerStartLocation* playerStart;

	//virtual void RestartPlayer(AController* player)override;


};



