// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ControllableInterface.h"
#include "RollingBall.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class BALLTUTORIAL_API ARollingBall : public APawn, public IControllableInterface
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ARollingBall();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UStaticMeshComponent* base;
	USpringArmComponent* spring;
	UCameraComponent* camera;
	UPROPERTY(VisibleAnywhere,BlueprintReadonly)
	UStaticMeshComponent* base;

	void timeout(float duration);
    UFUNCTION(BlueprintCallable, Category = "Checkers")
	void checkpoint();

	void moveRight(float AxisValue) override;
	void moveLeft(float AxisValue) override;
	void moveForward(float AxisValue) override;
	void moveBack(float AxisValue) override;
	void rotateRight(float AxisValue) override;
	void rotateLeft(float AxisValue) override;
	void jump() override;
	void slow(float AxisValue) override;
	void attack(float AxisValue) override;
	void boost(float AxisValue) override;

    UFUNCTION(BlueprintNativeEvent, Category = "Checkers")
    bool jumpCheck();
    bool jumpCheck_Implementation();

	void laser();

	float m_right;
	float m_left;
	float m_forward;
	float m_back;
	float m_rotateRight;
	float m_rotateLeft;
	float m_slow;
	float m_boost;
	float m_timeout;
	float m_attack;
	float m_reload;
	float m_reloadLeft;

	UPROPERTY(BlueprintReadWrite)
	FVector m_checkpoint;

	UPROPERTY(EditAnywhere)
    float m_gripDepth = 15.0;

	UPROPERTY(EditAnywhere)
    float m_gripWidth = 50.0;

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerSetPosition(FTransform position);
	bool ServerSetPosition_Validate(FTransform position);
	void ServerSetPosition_Implementation(FTransform position);

	UFUNCTION( NetMulticast, Unreliable,  WithValidation)
	void ClientSetPosition(FTransform position);
    bool ClientSetPosition_Validate(FTransform position);
    void ClientSetPosition_Implementation(FTransform position);
};
