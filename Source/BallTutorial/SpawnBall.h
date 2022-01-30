// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnBall.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API USpawnBall : public UAbility
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnBall();
	virtual void Use(FVector Direction) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere)
    float ProjectileSpeed = 2000.0f;
	UPROPERTY(EditAnywhere)
    float Reload = 3.0f;
    float TimeLastFire = 0.0f;
	UPROPERTY(EditAnywhere)
    FVector ProjectileOffset = FVector(100.0f,0.0f,100.0f);
	UPROPERTY(EditAnywhere)
    FRotator ProjectileRotation = FRotator(0.0f,0.0f,0.0f);

    AActor * Owner;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
