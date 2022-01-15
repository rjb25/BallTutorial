// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Jump.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UJump : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJump();
    void Jump();
    UPROPERTY(EditAnywhere)
    float JumpForce = 1000.0f;
    UPROPERTY(EditAnywhere)
    float GripWidth = 40.0f;
    UPROPERTY(EditAnywhere)
    float GripDepth = 10.0f;
    UPROPERTY(EditAnywhere)
    float JumpReload = 0.2f;
private:
    float LastJump = 0.0f;
    UStaticMeshComponent * Body = nullptr;
    AActor * Owner = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
