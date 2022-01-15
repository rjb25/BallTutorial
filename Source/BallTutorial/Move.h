// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Move.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UMove : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMove();
    void Move(FVector Direction, float DeltaTime,float Boost);
    UPROPERTY(EditAnywhere)
    float SpeedForce;
    UPROPERTY(EditAnywhere)
    float SpeedRoll;
    UPROPERTY(EditAnywhere)
    float BoostMult = 3.0f;
private:
    AActor * Owner;
    UStaticMeshComponent * Body;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
