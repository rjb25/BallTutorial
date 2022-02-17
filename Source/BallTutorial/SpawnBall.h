// Fill out your copyright notice in the Description page of Project Settings.
//if(Owner->controller = world->controller) then we know to do this. Otherwise check authority
//Will need one for multicast to spawn bots ammo and server players ammo also to spawn replicas of client side bullets
//client side bullets will know to spawn on the client because bullets will have a reference to their shooter? so if local player controller == my shooter player controller don't spawn?

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawn(FVector position);
	bool ServerSpawn_Validate(FVector position);
	void ServerSpawn_Implementation(FVector position);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
