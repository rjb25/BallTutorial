// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ARacingGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 3.0f;
	float Health = 0.0f;
	ARacingGameMode* GameModeRef;
	AActor* owner;

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void Suffer(float Damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
