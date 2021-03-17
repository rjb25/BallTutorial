// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RacingGameState.generated.h"

UCLASS()
class BALLTUTORIAL_API ARacingGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void StartRace();
	bool StartRace_Validate();
	void StartRace_Implementation();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void StopRace();
	bool StopRace_Validate();
	void StopRace_Implementation();

	//These values are set by blueprints in the locations folder.
	UPROPERTY(BlueprintReadWrite)
	FVector m_raceStart;

	UPROPERTY(BlueprintReadWrite)
	FVector m_lobbySpawn;
};
