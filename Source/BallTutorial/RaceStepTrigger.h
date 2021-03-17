// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceStepTrigger.generated.h"

UCLASS()
class BALLTUTORIAL_API ARaceStepTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ARaceStepTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;

	UPROPERTY(EditAnywhere)
	int m_step;

};
