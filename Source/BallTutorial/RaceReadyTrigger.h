// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceReadyTrigger.generated.h"

UCLASS()
class BALLTUTORIAL_API ARaceReadyTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ARaceReadyTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;
};
