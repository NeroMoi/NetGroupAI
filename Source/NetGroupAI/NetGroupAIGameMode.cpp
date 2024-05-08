// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetGroupAIGameMode.h"
#include "NetGroupAICharacter.h"
#include"PlayerStartLocation.h"
#include"GroupAIPlayerController.h"
#include"PlayerCharacter.h"
#include"Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ANetGroupAIGameMode::ANetGroupAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	

}

void ANetGroupAIGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{

		
		playerStart = World->SpawnActor<APlayerStartLocation>(FVector(0,0,0),FRotator::ZeroRotator);

		//playerStart->playerLocation1 = FVector(20.0f, 0.f, 20.f);
	
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Value:%f"), (playerStart->playerLocation2).X));

	}

}
