// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ObserverPlayer.generated.h"

UCLASS(Blueprintable)
class NETGROUPAI_API AObserverPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObserverPlayer();


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		USphereComponent* Shape;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UStaticMeshComponent* SM_Shape;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Test ObjectInstance")
		float value;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,CateGory = "RotationRate")
	 FQuat ActorRotationRate = (FQuat(FRotator(0, 1, 0)));


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
