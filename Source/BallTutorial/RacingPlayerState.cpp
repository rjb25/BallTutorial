// Fill out your copyright notice in the Description page of Project Settings.

//This is serverSide code
#include "RacingPlayerState.h"
#include "RacingGameMode.h"
#include "RacingHUD.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "RaceStepTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "RollingPlayerController.h"
#include "Components/TextBlock.h"
#include "RacingWidget.h"
#include "RollingBall.h"
#include "RacingGameState.h"
#include"Components/PrimitiveComponent.h"
ARacingPlayerState::ARacingPlayerState() {
	m_isReady = false;
	m_isDone = false;
	m_maxLap = 2;
	m_step = 0;
	m_lap = 0;
	m_totalSteps = 4;
	m_totalTime = 0;
	PrimaryActorTick.bCanEverTick = true;
}
// Called every frame
void ARacingPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(m_timerGoing){
		m_totalTime += DeltaTime;
		m_racingWidget->setTime(m_totalTime);
	}

}
void ARacingPlayerState::startTimer() {
	m_totalTime = 0;
	m_racingWidget = Cast<URacingWidget>(Cast<ARacingHUD>(Cast<APlayerController>(GetPawn()->GetController())->GetHUD())->m_raceStatsWidget);
	m_timerGoing = true;
}

void ARacingPlayerState::stopTimer() {
	m_timerGoing = false;


}
bool ARacingPlayerState::serverSetPlayerDone_Validate(bool isDone, float time) {
	return true;
}

void ARacingPlayerState::serverSetPlayerDone_Implementation(bool isDone, float time) {
	m_totalTime = time;
	m_isDone = isDone;
	UWorld * world = GetWorld();
	ARacingGameMode * game = Cast<ARacingGameMode>(world->GetAuthGameMode());
	game->checkAllDone();
}

bool ARacingPlayerState::serverSetPlayerReady_Validate(bool isReady) {
	return true;
}

void ARacingPlayerState::serverSetPlayerReady_Implementation(bool isReady) {
	m_isReady = isReady;
	UWorld * world = GetWorld();
	ARacingGameMode * game = Cast<ARacingGameMode>(world->GetAuthGameMode());
	game->checkAllReady();
}

bool ARacingPlayerState::clientSetPlace_Validate(int place) {
	return true;

}
void ARacingPlayerState::clientSetPlace_Implementation(int place) {
	m_racingWidget->setPlace(place);
}

bool ARacingPlayerState::getPlayerDone() {
	return m_isDone;
}

bool ARacingPlayerState::getPlayerReady() {
	return m_isReady;
}

//I don't need to replicate as I am specifically setting these values on the server through rpcs. However this is a nice example.
void ARacingPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARacingPlayerState, m_isReady);
	DOREPLIFETIME(ARacingPlayerState, m_isDone);
}

void ARacingPlayerState::BeginPlay() {
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARaceStepTrigger::StaticClass(), FoundActors);
	m_totalSteps = FoundActors.Num();
}

void ARacingPlayerState::setStep(int step) {
	if (m_step == m_totalSteps && step == 1) {
		m_step = step;
		m_lap++;
		m_racingWidget->setLap(m_lap);
	}
	if (m_step == step - 1) {
		m_step = step;
	}
	if (m_lap == m_maxLap) {
		stopTimer();
		serverSetPlayerDone(true,m_totalTime);
		ARollingBall * pawn = Cast<ARollingBall>(GetPawn());
		if (pawn->IsLocallyControlled()) {
			pawn->SetActorLocation(Cast<ARacingGameState>(GetWorld()->GetGameState())->m_lobbySpawn);
			Cast<UPrimitiveComponent>(pawn->base)->SetPhysicsLinearVelocity(FVector(0,0,0));
			Cast<UPrimitiveComponent>(pawn->base)->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
		}
		m_lap = 0;
	    m_racingWidget->setLap(m_lap);
		m_step = 0;
	}
}
