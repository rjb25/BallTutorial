// Fill out your copyright notice in the Description page of Project Settings.

#include "PointerLocationTest.h"

// Sets default values for this component's properties
UPointerLocationTest::UPointerLocationTest()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    TimeSinceLastPrint = 0;
    PrintSpeed = 5.0f;

	// ...
}


// Called when the game starts
void UPointerLocationTest::BeginPlay()
{
	Super::BeginPlay();
    
    FVector TempMyLocation = GetOwner()->GetActorLocation();
    MyLocation = &TempMyLocation;

	// ...
	
}


// Called every frame
void UPointerLocationTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TimeSinceLastPrint += DeltaTime;
    if(TimeSinceLastPrint > PrintSpeed){
        
GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("Current %s"), *MyLocation->ToString()));
TimeSinceLastPrint-=PrintSpeed;
    }


	// ...
}

