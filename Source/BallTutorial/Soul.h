// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soul.generated.h"
class AModularPlayerController;

UENUM()
enum Trait
{
    Player     UMETA(DisplayName = "Player"),
    Bot        UMETA(DisplayName = "Bot"),
    Neutral    UMETA(DisplayName = "Neutral"),
};

UCLASS()
class BALLTUTORIAL_API ASoul : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoul();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    UPROPERTY(BlueprintReadWrite)
    AModularPlayerController * PlayerController = nullptr;
};
