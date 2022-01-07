// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActableInterface.h"
#include "SpawnBall.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API USpawnBall : public UActorComponent, public IActableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnBall();
	void act() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere)
    float m_speed;
	UPROPERTY(EditAnywhere)
    float m_reload;
    float m_timeLastFire;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
