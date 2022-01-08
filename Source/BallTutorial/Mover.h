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
    UPROPERTY(EditAnywhere)
    FVector Destination;

    UPROPERTY(EditAnywhere)
    float TimeTo;

    UPROPERTY(EditAnywhere)
    float PauseAfter;

    UPROPERTY(EditAnywhere)
    bool Relative = false;
};

struct StateSet
{
    TArray<FToState>
    States;
    int StateIndex;
    float TimeOfNextIndex;
    bool HasStates;
    FVector From;
    FVector Current;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UMover();
    AActor * Owner;
    UStaticMeshComponent * Body;
    UPROPERTY(EditAnywhere)
    TArray<FToState> LocationChanges;

    UPROPERTY(EditAnywhere, Category = "Index")
    int LocationChangesStartIndex;

    UPROPERTY(EditAnywhere)
    TArray<FToState> RotationChanges;

    UPROPERTY(EditAnywhere, Category = "Index")
    int RotationChangesStartIndex;

    UPROPERTY(EditAnywhere)
    TArray<FToState> SizeChanges;

    UPROPERTY(EditAnywhere, Category = "Index")
    int SizeChangesStartIndex;

    UPROPERTY(EditAnywhere)
    bool Reverse;

    TArray<StateSet> StateSets;

    float TimeTotal;

    FVector ActorScale;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
