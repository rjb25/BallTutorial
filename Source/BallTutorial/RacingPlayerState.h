// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RacingPlayerState.generated.h"

/**
 * 
 */
class UWorld;
class URacingWidget;
UCLASS()
class BALLTUTORIAL_API ARacingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	ARacingPlayerState();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable, WithValidation)
	void clientSetPlace(int place);
	void clientSetPlace_Implementation(int place);
	bool clientSetPlace_Validate(int place);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void serverSetPlayerReady(bool isReady);
	void serverSetPlayerReady_Implementation(bool isReady);
	bool serverSetPlayerReady_Validate(bool isReady);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void serverSetPlayerDone(bool isDone, float time);
	void serverSetPlayerDone_Implementation(bool isDone, float time);
	bool serverSetPlayerDone_Validate(bool isDone, float time);

	bool getPlayerReady();

	bool getPlayerDone();
	void setStep(int step);
	void incTotalSteps();
	void startTimer();
	void stopTimer();

	UPROPERTY(Replicated)
	bool m_isDone;
	UPROPERTY(Replicated)
	bool m_isReady;
	int m_step;
	UPROPERTY(BlueprintReadOnly)
	int m_lap;
	int m_maxLap;
	int m_timerGoing;
	UPROPERTY(BlueprintReadOnly)
	float m_totalTime;
	int m_totalSteps;
	//int m_place;

	UPROPERTY()
	URacingWidget * m_racingWidget;
};
