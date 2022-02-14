// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransformMover.generated.h"

USTRUCT(BlueprintType)
struct BALLTUTORIAL_API FToTransform
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,meta=(MakeEditWidget))
    FTransform RelativeDestination;

    FTransform GlobalDestination;

    UPROPERTY(EditAnywhere)
    AActor * ActorAsDestination = nullptr;

    UPROPERTY(EditAnywhere)
    float TimeTo = 0.0f;

    UPROPERTY(EditAnywhere)
    float PauseAfter = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UTransformMover : public USceneComponent
{
	GENERATED_BODY()

public:	

	UTransformMover();
    AActor * Owner = nullptr;
    UStaticMeshComponent * Body = nullptr;

    UPROPERTY(EditAnywhere)
    TArray<FToTransform> ToTransforms;

    FTransform Reference;

    int ToIndex = 0;
    int FromIndex = 0;
    int IndexDirection = 1;

    UPROPERTY(EditAnywhere)
    bool ReverseNotLoop = false;

    UPROPERTY(EditAnywhere)
    bool ActorLocation = true;
    UPROPERTY(EditAnywhere)
    bool ActorRotation = false;
    UPROPERTY(EditAnywhere)
    bool ActorScale = false;
    //If you ever find yourself needing something to change to a specific size while moving lol
    //Technically this should also be on each item, but that's a lot of bloat

    float TimeTotal = 0.0f;
    float TimeOfChange = 0.0f;
    float TimeToDestination = 0.0f;
    float PauseAfterDestination = 0.0f;

	UFUNCTION( NetMulticast, Reliable,  WithValidation)
	void AllUpdate(int ServerFromIndex, int ServerToIndex, int ServerIndexDirection);
    bool AllUpdate_Validate(int ServerFromIndex, int ServerToIndex, int ServerIndexDirection);
    void AllUpdate_Implementation(int ServerFromIndex, int ServerToIndex, int ServerIndexDirection);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
