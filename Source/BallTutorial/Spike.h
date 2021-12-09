// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spike.generated.h"

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API USpike : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpike();
	AActor * Owner;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void DealDamage(AActor* OverlappedActor, AActor* OtherActor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
