// Fill out your copyright notice in the Description page of Project Settings.

#include "RacingGameState.h"
#include "RacingPlayerState.h"
#include "RollingBall.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"

bool ARacingGameState::StartRace_Validate() {
	return true;
}

void ARacingGameState::StartRace_Implementation() {
	for (int i = 0; i < PlayerArray.Num(); i++) {
		ARacingPlayerState * racer = Cast<ARacingPlayerState>(PlayerArray[i]);
		ARollingBall * pawn = Cast<ARollingBall>(PlayerArray[i]->GetPawn());
		if (pawn->IsLocallyControlled()) {
			pawn->SetActorLocation(m_raceStart);
			Cast<UPrimitiveComponent>(pawn->base)->SetPhysicsLinearVelocity(FVector(0,0,0));
			Cast<UPrimitiveComponent>(pawn->base)->SetPhysicsAngularVelocityInDegrees(FVector(0,0,0));
			USpringArmComponent * springy = pawn->spring;
			USceneComponent * prim = Cast<USceneComponent>(springy);
			FRotator currentRotation = prim->GetComponentRotation();
			currentRotation.Yaw = 180.0f;
			prim->SetWorldRotation(currentRotation);

			racer->startTimer();

		}
	}
}

bool ARacingGameState::StopRace_Validate() {
	return true;
}

void ARacingGameState::StopRace_Implementation() {
	//Show fireworks/whatever. Nothing here though because I handle race end in gamemode/playerstate
}
