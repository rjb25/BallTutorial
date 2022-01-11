// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBase.h"
#include "ActableInterface.h"
#include "Components/SphereComponent.h"
#include "EnemyAIBasic.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UEnemyAIBasic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAIBasic();
    UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SensoryBoxToSpawn;
    UPROPERTY(EditAnywhere)
	AActor * SensoryBox;
    AActor * Owner;
    UPROPERTY(BlueprintReadWrite, meta = (MustImplement = "ActableInterface"))
    UActorComponent * Action;
    FVector StartLocation;
    TArray<AActor*> Targets;
    UPROPERTY(EditAnywhere)
    float Speed;
    UPROPERTY(EditAnywhere)
    FVector SensoryScale;
    FVector Direction;
    UPROPERTY(EditAnywhere)
    bool Enabled;
    AActor * OldestTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) ;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
