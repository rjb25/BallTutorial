// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnBall.generated.h"

class UHealthComponent;

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
    float Reload = 3.0f;
	UPROPERTY(EditAnywhere)
    bool ClientSideHit = true;
    float TimeLastFire = 0.0f;
	UPROPERTY(EditAnywhere)
    FVector ProjectileOffset = FVector(100.0f,0.0f,100.0f);
	UPROPERTY(EditAnywhere)
    FRotator ProjectileRotation = FRotator(0.0f,0.0f,0.0f);

    AActor * Owner = nullptr;
    UHealthComponent * HealthComp = nullptr;

    /*
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawn(FTransform position);
	bool ServerSpawn_Validate(FTransform position);
	void ServerSpawn_Implementation(FTransform position);

	UFUNCTION( NetMulticast, Reliable,  WithValidation)
	void ClientSpawn(FTransform position);
    bool ClientSpawn_Validate(FTransform position);
    void ClientSpawn_Implementation(FTransform position);
    */

    void Spawn(FVector Direction);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
