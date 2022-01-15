// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slow.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API USlow : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlow();
    void Slow(float DeltaTime);
    UPROPERTY(EditAnywhere)
    float SlowForce = 1000.0f;
    UPROPERTY(EditAnywhere)
    float SlowTorque = 1000.0f;
    UPROPERTY(EditAnywhere)
    float GripWidth = 40.0f;
    UPROPERTY(EditAnywhere)
    float GripDepth = 10.0f;

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
