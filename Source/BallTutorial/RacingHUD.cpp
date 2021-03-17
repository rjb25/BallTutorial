// Fill out your copyright notice in the Description page of Project Settings.

#include "RacingHUD.h"
#include "Blueprint/UserWidget.h"
#include "RacingPlayerState.h"
#include "RollingPlayerController.h"
#include "RacingWidget.h"
#include "RacingGameState.h"

ARacingHUD::ARacingHUD() {

}
void ARacingHUD::BeginPlay() {
	Super::BeginPlay();
	createWidgets();
}

void ARacingHUD::createWidgets() {
		m_raceStatsWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), m_raceStats);
		if (m_raceStatsWidget) {
		    m_raceStatsWidget->AddToViewport();
		}
}

