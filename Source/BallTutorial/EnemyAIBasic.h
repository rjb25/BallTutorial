// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBase.h"
#include "CanActInterface.h"
#include "ActableInterface.h"
#include "Components/SphereComponent.h"
#include "EnemyAIBasic.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UEnemyAIBasic : public UActorComponent, public ICanActInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAIBasic();
    IActableInterface * mItem;
    UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SensoryBoxToSpawn;
    UPROPERTY(EditAnywhere)
	AActor * SensoryBox;
    AActor * Owner;
    FVector StartLocation;
    TArray<AActor*> Targets;
    UPROPERTY(EditAnywhere)
    float Speed;
    UPROPERTY(EditAnywhere)
    FVector SensoryScale;

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

	void act() override;
	void setAct(IActableInterface * item) override;
};
