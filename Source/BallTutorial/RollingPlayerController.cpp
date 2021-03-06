// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingPlayerController.h"
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
