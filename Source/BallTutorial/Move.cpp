// Fill out your copyright notice in the Description page of Project Settings.

#include "Move.h"
#include "AdventureGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMove::UMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    SpeedForce = 3000.0f;
    SpeedRoll = 150000000.0f;
    Owner = nullptr;
    Body = nullptr;

	// ...
}


// Called when the game starts
void UMove::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();
    if (Body != nullptr){
	Body->SetSimulatePhysics(true);
	Body->SetNotifyRigidBodyCollision(true);
	Body->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
    } else {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CHICKEN BEFORE THE EGG IN MOVE.cpp")));
    }
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                FString::Printf(TEXT("Force Roll %f %f"), SpeedForce, SpeedRoll));
}

void UMove::Move(FVector Direction, float DeltaTime, float Boost){
        //Force for movement
    float MoveForce; 
    if(Boost > 0.9f){
        MoveForce = DeltaTime *  SpeedForce * Body->GetMass() * BoostForceMult;
    }else{
        MoveForce = DeltaTime *  SpeedForce * Body->GetMass();
    }
    Body->AddForce(Direction * MoveForce);
    FVector Velocity = Owner->GetVelocity();
    if(Velocity.Z < 0.0f){
        Velocity.Z = 0.0f;
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Velocity %f"),Velocity.Size()));
    if(Velocity.Size() > MaximumVelocity + Boost * BoostMaxAdd){
        Velocity.Z = 0.0f;
        FVector OppositeVelocityDirection = -1 * 
            UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                    Velocity);
        Body->AddForce(OppositeVelocityDirection * MoveForce);
    }

    /*
    FVector AngularVelocity = Body->GetPhysicsAngularVelocityInDegrees();
    if(AngularVelocity.Size() > 10.0f){
    FVector UnitAngularDirection = 
        UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                AngularVelocity);
    FVector DragAngular = -1 * UnitAngularDirection * (AngularVelocity * AngularVelocity) * DragTorque;
    Body->AddTorqueInDegrees(DeltaTime * DragAngular * Body->GetMass());
        FVector FlatDragAngular = -1 * UnitAngularDirection * FlatDragTorque;
        Body->AddTorqueInDegrees(DeltaTime * FlatDragAngular * Body->GetMass());
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                FString::Printf(TEXT("Torque %s %f"), *(DeltaTime * FlatDragAngular * Body->GetMass()).ToString(), FlatDragTorque));
    }
    */

    //Rotational torque for movement
    FVector RotationDirection;
    RotationDirection.Y = Direction.X;
    RotationDirection.X = Direction.Y * (-1);
    RotationDirection.Z = Direction.Z;
    if(Boost > 0.9f){
        Body->AddTorqueInDegrees(DeltaTime * RotationDirection * SpeedRoll * Body->GetMass());
    }else{
        Body->AddTorqueInDegrees(DeltaTime * RotationDirection * SpeedRoll * Body->GetMass());
    }
}


// Called every frame
void UMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

