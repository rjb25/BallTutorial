// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControllerInterface.h"
#include "RollingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API ARollingPlayerController : public APlayerController, public IControllerInterface
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
	void act(float AxisValue);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerHurt(AActor* toHurt, float pain) override;
	bool ServerHurt_Validate(AActor* toHurt, float pain) override;
	void ServerHurt_Implementation(AActor* toHurt, float pain) override;
};
