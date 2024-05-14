// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "ObserverPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class NETGROUPAI_API AObserverPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD()override;


protected:

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* CrosshairTexture;




	
};
