// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Move.h"
#include "ModularPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void SetPawn(APawn* InPawn) override;

public:
	AModularPlayerController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	float Right;
	float Left;
	float Forward;
	float Back;
	float RotateRight;
	float RotateLeft;
	float Slow;
	float Boost;
	float Attack;
    UMove * Movement;
	
};
