// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AdventureSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API UAdventureSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FVector PlayerLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector PlayerCheckpoint;
	UPROPERTY(BlueprintReadWrite)
	bool newGame = true;
	
};
