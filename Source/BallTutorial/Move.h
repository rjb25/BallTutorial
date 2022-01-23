// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Move.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UMove : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMove();
    void Move(FVector Direction, float DeltaTime, bool Boost = false, bool Slow = false);
    UPROPERTY(EditAnywhere, Category = "Force")
    float SpeedForce = 3000000.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float SpeedTorque = 300000000.0;
    

    UPROPERTY(EditAnywhere, Category = "Force")
    float BoostForceMult = 2.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float BoostTorqueMult = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float SlowForceMult = 1.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float SlowTorqueMult = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float AirForceMult = 0.3f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float AirTorqueMult = 0.3f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float BoostMaxAddForce = 400.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float BoostMaxAddTorque = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float SlowMaxAddForce = -300.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float SlowMaxAddTorque = -300.0f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float AirMaxAddForce = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float AirMaxAddTorque = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Force")
    float MaximumVelocityForce = 400.0f;
    UPROPERTY(EditAnywhere, Category = "Torque")
    float MaximumVelocityTorque = 400.0f;
/*
    UPROPERTY(EditAnywhere, Category = "Force")
    float GripForce = 1000.0f;
*/
    //So we don't have the ball spinning while still
    UPROPERTY(EditAnywhere, Category = "Torque")
    float AntiTwirlGrip = 100000000.0f;

    UPROPERTY(EditAnywhere)
    float GripWidth = 40.0f;
    UPROPERTY(EditAnywhere)
    float GripDepth = 30.0f;

    UPROPERTY(EditAnywhere)
    float ExtraGravity = 10000.0f;

    UStaticMeshComponent * Body = nullptr;

private:
    AActor * Owner = nullptr;
    void Grip(float DeltaTime);
    void GripVelocity(float DeltaTime);
    void GripAngularVelocity(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
