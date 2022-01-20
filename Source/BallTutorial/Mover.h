// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"

USTRUCT(BlueprintType)
struct BALLTUTORIAL_API FToState 
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,meta=(MakeEditWidget))
    FVector Destination;
    FVector SaveRelative;

    UPROPERTY(EditAnywhere)
    bool Relative = false;

    UPROPERTY(EditAnywhere)
    AActor * ActorAsDestination = nullptr;

    UPROPERTY(EditAnywhere)
    float TimeTo;

    UPROPERTY(EditAnywhere)
    float PauseAfter;

};

USTRUCT()
struct FStateSet
{
    GENERATED_BODY()

public:
    //Pointer to LocationChanges, RotationChanges, or SizeChanges array
    TArray<FToState> *
    States;
    int StateIndex;
    int PriorStateIndex;
    float TimeOfNextIndex;
    int IndexDirection = 1;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UMover : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UMover();
    AActor * Owner;
    UStaticMeshComponent * Body;
    UPROPERTY(EditAnywhere)
    TArray<FToState> LocationChanges;
    UPROPERTY(EditAnywhere)
    TArray<FToState> RotationChanges;
    UPROPERTY(EditAnywhere)
    TArray<FToState> SizeChanges;
    UPROPERTY(EditAnywhere)
    bool UpdateActorDestinations;

    UPROPERTY(EditAnywhere)
    bool ReverseNotLoopLocation;
    UPROPERTY(EditAnywhere)
    bool ReverseNotLoopRotation;
    UPROPERTY(EditAnywhere)
    bool ReverseNotLoopSize;

    TArray<bool*> Reverses;

    TArray<FStateSet*> StateSets;
    FStateSet LocationSet;
    FStateSet RotationSet;
    FStateSet SizeSet;

    float TimeTotal;
    bool structbool = false;

    FVector ActorScale;

    UPROPERTY(EditAnywhere)
    float TimeToStartLocation;
    UPROPERTY(EditAnywhere)
    float TimeToStartRotation;
    UPROPERTY(EditAnywhere)
    float TimeToStartSize;

    UPROPERTY(EditAnywhere)
    float PauseAfterStartLocation;
    UPROPERTY(EditAnywhere)
    float PauseAfterStartRotation;
    UPROPERTY(EditAnywhere)
    float PauseAfterStartSize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
