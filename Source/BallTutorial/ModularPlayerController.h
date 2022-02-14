// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

class UMove;
class UJump;
class UAbility;
class UJoint;
class UCameraComponent;
class UHealthComponent;

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void AcknowledgePossession(APawn* InPawn) override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	void Possessed(APawn* InPawn);
	AModularPlayerController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHurt(AActor* toHurt, float pain, TSubclassOf<UDamageType> DamageType);
	bool ServerHurt_Validate(AActor* toHurt, float pain, TSubclassOf<UDamageType> DamageType);
	void ServerHurt_Implementation(AActor* toHurt, float pain, TSubclassOf<UDamageType> DamageType);

	void RightAxis(float AxisValue);
	void ForwardAxis(float AxisValue);
	void RotateAxis(float AxisValue);
	void NodAxis(float AxisValue);
	void JumpPressed();
	void JumpReleased();
	//void GetYouOne();
	//void possess();
	void BoostPressed();
	void BoostReleased();
	void SlowPressed();
	void SlowReleased();
	void AbilityPressed();
	void AbilityReleased();

	void menu();
	//void TryPossess(APawn* PossessMe);

	void ToCheckpoint();
	UPROPERTY(BlueprintReadWrite)
    FVector Checkpoint;

	float Right = 0.0f;
	float Forward = 0.0f;
	bool Boost = false;
	bool Slow = false;
	bool Jump = false;
	bool Ability = false;
    UMove * MovementComp = nullptr;
    UJoint * JointComp = nullptr;
    UJump * JumpComp = nullptr;
    UCameraComponent * CameraComp = nullptr;
    UHealthComponent * HealthComp = nullptr;
    UPROPERTY()
    TArray<UAbility*> AbilityComps;
    AActor * Actor = nullptr;
    UStaticMeshComponent * Body = nullptr;
    UPrimitiveComponent * Primitive = nullptr;

    APawn * Pawn = nullptr;
    ACharacter * Character = nullptr;
    UStaticMeshComponent * MarkerBody = nullptr;

private:
    bool FirstPossession = true;
};
