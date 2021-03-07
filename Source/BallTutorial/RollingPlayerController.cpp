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
	Cast<IControllableInterface>(GetPawn())->moveRight(AxisValue);
}
void ARollingPlayerController::moveLeft(float AxisValue) {
	Cast<IControllableInterface>(GetPawn())->moveLeft(AxisValue);
}
void ARollingPlayerController::moveForward(float AxisValue) {
	Cast<IControllableInterface>(GetPawn())->moveForward(AxisValue);
}
void ARollingPlayerController::moveBack(float AxisValue) {
	Cast<IControllableInterface>(GetPawn())->moveBack(AxisValue);
}
void ARollingPlayerController::rotateRight(float AxisValue) {
	Cast<IControllableInterface>(GetPawn())->rotateRight(AxisValue);
}
void ARollingPlayerController::rotateLeft(float AxisValue) {
	Cast<IControllableInterface>(GetPawn())->rotateLeft(AxisValue);
}
