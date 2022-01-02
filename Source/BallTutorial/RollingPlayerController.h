// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RollingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API ARollingPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void SetupInputComponent() override;

	void moveRight(float AxisValue);
	void moveLeft(float AxisValue);
	void moveForward(float AxisValue);
	void moveBack(float AxisValue);
	void rotateRight(float AxisValue);
	void rotateLeft(float AxisValue);
	void jump();
	void menu();
	void boost(float AxisValue);
	void slow(float AxisValue);
	void attack(float AxisValue);
};
