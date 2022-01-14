// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Move.h"
#include "Jump.h"
#include "SpawnBall.h"
#include "ControllerInterface.h"
#include "ModularPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API AModularPlayerController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void AcknowledgePossession(APawn* InPawn) override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	void Possessed(APawn* InPawn);
	AModularPlayerController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void SetupInputComponent() override;

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerHurt(AActor* toHurt, float pain) override;
	bool ServerHurt_Validate(AActor* toHurt, float pain) override;
	void ServerHurt_Implementation(AActor* toHurt, float pain) override;

	void moveRight(float AxisValue);
	void moveLeft(float AxisValue);
	void moveForward(float AxisValue);
	void moveBack(float AxisValue);
	void rotateRight(float AxisValue);
	void rotateLeft(float AxisValue);
	void JumpPressed();
	void JumpReleased();
	void menu();
	void boost(float AxisValue);
	void slow(float AxisValue);
	void attack(float AxisValue);
	void act(float AxisValue);

    UPROPERTY(EditAnywhere)
    float CameraTurnSpeed = 1.0f;
	float Right = 0.0f;
	float Left = 0.0f;
	float Forward = 0.0f;
	float Back = 0.0f;
	float RotateRight = 0.0f;
	float RotateLeft = 0.0f;
	float Slow = 0.0f;
	float Act = 0.0f;
	bool Jump = false;
	float Boost = 0.0f;
	float Attack = 0.0f;
    UMove * MovementComp = nullptr;
    UJump * JumpComp = nullptr;
    USceneComponent * SpringComp = nullptr;
    USpawnBall * SpawnBallComp = nullptr;
	
};
