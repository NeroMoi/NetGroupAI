// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStartLocation.h"

// Sets default values
APlayerStartLocation::APlayerStartLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerLocation1 = FVector(20.0f, 0.f, 20.f);
	playerLocation2 = FVector(0, 0, 0);

}

FVector APlayerStartLocation::GenerateStartLocation()
{
	auto playerNum = GetWorld()->GetNumControllers();

	 playerLocation1 += FVector(10 * playerNum, 10 * playerNum, 0);

	 return playerLocation1;
}

// Called when the game starts or when spawned
void APlayerStartLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerStartLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

