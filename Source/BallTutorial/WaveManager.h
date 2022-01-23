// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

class ASoul;

USTRUCT()
struct BALLTUTORIAL_API FSpawnable 
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int LocationIndex = 0;

    UPROPERTY(EditAnywhere)
    bool MustDie = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASoul> SoulToSpawn = nullptr;
};
USTRUCT()
struct BALLTUTORIAL_API FSwitchable
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool DefaultShow = false;

    UPROPERTY(EditAnywhere)
    bool Visible = false;

	UPROPERTY(EditAnywhere)
	AActor * ActorToManipulate = nullptr;

    bool HiddenDefault = false;
    bool TickDefault = false;
    bool CollisionDefault = false;
};

/*
USTRUCT()
struct BALLTUTORIAL_API FDropable 
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int LocationIndex = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DropToSpawn = nullptr;
};
*/

USTRUCT()
struct BALLTUTORIAL_API FWave
{
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
    TArray<FSpawnable> Spawns;
	UPROPERTY(EditAnywhere)
    TArray<FSwitchable> Switches;
    /*
	UPROPERTY(EditAnywhere)
    TArray<FDropable> Drops;
    */
};

UCLASS()
class BALLTUTORIAL_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    void Show(FSwitchable * Switch);
    void Hide(FSwitchable * Switch);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    TArray<FWave> Waves;

    TArray<ASoul*> NeededDeaths;

    UPROPERTY(EditAnywhere,meta=(MakeEditWidget))
    TArray<FVector> SpawnLocations;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent * mesh = nullptr;

    UPROPERTY(EditAnywhere)
    AActor * SensorBox = nullptr;

    int CurrentWave = 0;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) ;

	UFUNCTION()
    void SoulDied(AActor* DestroyedActor) ;
    int ActorsAlive = 0;

    void SpawnWave();
    void SpawnIt(FSpawnable Spawn);
    /*
    void DropIt(FDropable Drop);
    */


};
