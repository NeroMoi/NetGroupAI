// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerStartLocation.generated.h"

UCLASS()
class NETGROUPAI_API APlayerStartLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStartLocation();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		FVector playerLocation1;//玩家起始位置

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		FVector playerLocation2;//暂时无用

	UFUNCTION(BlueprintCallable)
		FVector GenerateStartLocation();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
