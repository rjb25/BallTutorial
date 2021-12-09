// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnComponentHitActor.generated.h"

class UBoxComponent;

UCLASS()
class BALLTUTORIAL_API AOnComponentHitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnComponentHitActor();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* MyComp;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
