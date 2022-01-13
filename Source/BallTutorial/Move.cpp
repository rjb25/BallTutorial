// Fill out your copyright notice in the Description page of Project Settings.

#include "Move.h"
#include "AdventureGameInstance.h"

// Sets default values for this component's properties
UMove::UMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    SpeedForce = 3000.0f;
    SpeedRoll = 1000000.0f;

	// ...
}


// Called when the game starts
void UMove::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();
	Body->SetSimulatePhysics(true);
	Body->SetNotifyRigidBodyCollision(true);
	Body->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
}

void UMove::Move(FVector Direction, float DeltaTime){
        //Force for movement
        Body->AddForce(DeltaTime * Direction * SpeedForce * Body->GetMass());

        //Rotational torque for movement
        FVector RotationDirection;
        RotationDirection.Y = Direction.X;
        RotationDirection.X = Direction.Y * (-1);
        Body->AddTorqueInRadians(DeltaTime * RotationDirection * SpeedRoll * Body->GetMass());
}


// Called every frame
void UMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

