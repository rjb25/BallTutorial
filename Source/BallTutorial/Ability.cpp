// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

// Sets default values for this component's properties
UAbility::UAbility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbility::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAbility::Use(FVector Direction)
{
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Use from Ability Base class called")));

}


// Called every frame
void UAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

