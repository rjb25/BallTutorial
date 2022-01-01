// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingPlayerController.h"
#include "RollingBall.h"
#include "Kismet/GameplayStatics.h"
#include "AdventureGameInstance.h"
#include "AdventureSaveGame.h"
#include "ControllableInterface.h"
// Called to bind functionality to input
void ARollingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("Right", this, &ARollingPlayerController::moveRight);
	InputComponent->BindAxis("Left", this, &ARollingPlayerController::moveLeft);
	InputComponent->BindAxis("Forward", this, &ARollingPlayerController::moveForward);
	InputComponent->BindAxis("Back", this, &ARollingPlayerController::moveBack);
	InputComponent->BindAxis("RotateRight", this, &ARollingPlayerController::rotateRight);
	InputComponent->BindAxis("RotateLeft", this, &ARollingPlayerController::rotateLeft);
	InputComponent->BindAxis("Slow", this, &ARollingPlayerController::slow);
	InputComponent->BindAxis("Boost", this, &ARollingPlayerController::boost);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARollingPlayerController::jump);
	InputComponent->BindAction("Menu", IE_Pressed, this, &ARollingPlayerController::menu);
}
void ARollingPlayerController::moveRight(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->moveRight(AxisValue);
	}
}
void ARollingPlayerController::moveLeft(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->moveLeft(AxisValue);
	}
}
void ARollingPlayerController::moveForward(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->moveForward(AxisValue);
	}
}
void ARollingPlayerController::moveBack(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->moveBack(AxisValue);
	}
}
void ARollingPlayerController::rotateRight(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->rotateRight(AxisValue);
	}
}
void ARollingPlayerController::rotateLeft(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->rotateLeft(AxisValue);
	}
}
void ARollingPlayerController::jump() {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->jump();
	}
}
void ARollingPlayerController::menu() {
    UAdventureGameInstance * game = Cast<UAdventureGameInstance>(GetGameInstance());
    UAdventureSaveGame * save = game->AdventureSave;
    save->newGame = false;
    //TODO Set player location and checkpoint
	ARollingBall * rollingBall = Cast<ARollingBall>(GetPawn());
    if(rollingBall != nullptr){
        save->PlayerLocation = rollingBall->GetActorLocation();
        save->PlayerCheckpoint =  rollingBall->m_checkpoint;
    }
    UGameplayStatics::SaveGameToSlot(save, game->AdventureSlot, 0);
}

void ARollingPlayerController::slow(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->slow(AxisValue);
	}
}

void ARollingPlayerController::boost(float AxisValue) {
	IControllableInterface * pawn = Cast<IControllableInterface>(GetPawn());
	if (pawn) {
		pawn->boost(AxisValue);
	}
}
