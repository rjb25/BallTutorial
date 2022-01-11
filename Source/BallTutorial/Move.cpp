// Fill out your copyright notice in the Description page of Project Settings.

#include "Move.h"

// Sets default values for this component's properties
UMove::UMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    SpeedForce = 3000.0f;
    SpeedRoll = 1000000.0f;

	// ...
}


// Called when the game starts
void UMove::BeginPlay()
{
	Super::BeginPlay();
    Body = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	Body->SetSimulatePhysics(true);
	Body->SetNotifyRigidBodyCollision(true);
	Body->BodyInstance.SetCollisionProfileName("BlockAllDynamic");


	// ...
	
}

void UMove::Move(FVector Direction, float DeltaTime){

        //Force for movement
        Body->AddForce(DeltaTime * Direction * SpeedForce * Body->GetMass());

        //Rotational torque for movement
        FVector RotationDirection;
        RotationDirection.Y = Direction.X;
        RotationDirection.X = Direction.Y * (-1);
        Body->AddTorqueInRadians(DeltaTime * RotationDirection * SpeedRoll * Body->GetMass());
    /*
    if (HasAuthority()) {
        ClientSetPosition(GetActorTransform());
    }
    //Send local positions to server which will then send our position to other clients
    else if (!HasAuthority() && IsLocallyControlled()) {
        ServerSetPosition(GetActorTransform());
    }

    if (IsLocallyControlled()) {
        //Directional push
        float side = m_right + m_left;
        float ahead = m_forward + m_back;
        FVector direction = { ahead, side, 0 };
        FVector nowhere = { 0,0,0 };
        FVector verticalAxis = { 0,0,1 };
        FVector unitDirection = UKismetMathLibrary::GetDirectionUnitVector(nowhere, direction);
        float pushScale = 3000.0;
        FVector rotatedUnitDirection = UKismetMathLibrary::RotateAngleAxis(unitDirection, springRotation.Yaw, verticalAxis);
        base->AddForce(DeltaTime * rotatedUnitDirection * mod * pushScale * base->GetMass());

        //Rotational torque for movement
        float swap = rotatedUnitDirection.Y;
        rotatedUnitDirection.Y = rotatedUnitDirection.X;
        rotatedUnitDirection.X = swap * (-1);
        float torqueScale = 1000000.0;
        base->AddTorqueInRadians(DeltaTime * rotatedUnitDirection * mod * torqueScale * base->GetMass());
    }
    UStaticMeshComponent * Body = Owner->FindComponentByClass<UStaticMeshComponent>();
    float Mass = Body->GetMass();
    Body->AddForce(Direction * Mass * Speed);
        //DestinationDirection = Targets.Num() > 0 ? Targets[0]->GetActorLocation() : StartLocation;
    */
}


// Called every frame
void UMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

