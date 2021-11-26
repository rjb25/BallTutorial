// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RacingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API ARacingGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARacingGameMode();
	bool checkAllReady();
	bool checkAllDone();
	void setRacerTime(float time);
	void ActorDied(AActor* DeadActor);
	
};
