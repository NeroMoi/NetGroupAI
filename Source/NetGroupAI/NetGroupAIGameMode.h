// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerStartLocation.h"
#include "Blueprint/UserWidget.h"
#include "NetGroupAIGameMode.generated.h"

UCLASS(minimalapi)
class ANetGroupAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANetGroupAIGameMode();

	virtual void BeginPlay();

	/** 移除当前菜单控件，并在指定类（如有）中新建控件。*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerStartLocation* playerStart;

	/** 游戏开始时，用作菜单的控件类。*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** 用作菜单的控件实例。*/
	UPROPERTY()
		UUserWidget* CurrentWidget;

	//virtual void RestartPlayer(AController* player)override;





};



