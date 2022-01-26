// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Joint.generated.h"

USTRUCT()
struct BALLTUTORIAL_API FJoinable
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FVector Offset = FVector(0,0,0);

    UPROPERTY(EditAnywhere)
    FRotator Rotation = FRotator(0,0,0);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn = nullptr;

    AActor * Actor;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UJoint : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJoint();
	UPROPERTY(EditAnywhere)
    TArray<FJoinable> Joinables;
    AActor * Owner = nullptr;
	UFUNCTION()
    void KillActor(AActor* ActorThatDied) ;
    FRotator CurrentRotation = FRotator(0,0,0);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
