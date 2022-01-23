// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAIBasic.generated.h"

class UJump;
class UMove;
class USpawnBall;

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UEnemyAIBasic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAIBasic();

    UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SensorBoxToSpawn = nullptr;

    TArray<AActor*> Targets;

    UPROPERTY(EditAnywhere)
	AActor * SensorBox = nullptr;
    AActor * OldestTarget = nullptr;
    AActor * Owner = nullptr;

    UMove * MovementComp = nullptr;
    UJump * JumpComp = nullptr;
    USpawnBall * SpawnBallComp = nullptr;
    UStaticMeshComponent * Body = nullptr;
    UPrimitiveComponent * Primitive = nullptr;

    UPROPERTY(EditAnywhere)
    FVector SensoryScale = FVector(1.0f, 1.0f, 1.0f);
    FVector StartLocation = FVector(0.0f,0.0f,0.0f);
    FVector Direction = FVector(0.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere)
    bool Enabled = true;
    UPROPERTY(EditAnywhere)
    bool JumpEvade = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) ;

    void SetSensorBox(AActor*);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
