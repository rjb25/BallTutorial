// Fill out your copyright notice in the Description page of Project Settings.

#include "Move.h"
#include "AdventureGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyUtil.h"

// Sets default values for this component's properties
UMove::UMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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
    /*
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                FString::Printf(TEXT("Force Roll %f %f"), SpeedForce, SpeedTorque));
                */
}

void UMove::Move(FVector Direction, float DeltaTime, bool Boost, bool Slow){
    bool Grounded = MyUtil::GroundCheck(Owner,GripDepth,GripWidth);


    bool ActiveMovement = !Direction.IsZero();
    if(ActiveMovement){
        float LBoostForceMult    = 1.0f;
        float LBoostTorqueMult   = 1.0f;
        float LAirForceMult      = 1.0f;
        float LAirTorqueMult     = 1.0f;
        float LSlowForceMult     = 1.0f;
        float LSlowTorqueMult    = 1.0f;

        float LBoostMaxAddForce  = 0.0f;
        float LBoostMaxAddTorque = 0.0f;
        float LSlowMaxAddForce   = 0.0f;
        float LSlowMaxAddTorque  = 0.0f;
        float LAirMaxAddForce    = 0.0f;
        float LAirMaxAddTorque   = 0.0f;

        if (Grounded){
            if (Boost){
                LBoostForceMult = BoostForceMult;
                LBoostTorqueMult = BoostTorqueMult;
                LBoostMaxAddForce = BoostMaxAddForce;
                LBoostMaxAddTorque = BoostMaxAddTorque;
            }

            if (Slow){
                LSlowForceMult = SlowForceMult;
                LSlowTorqueMult = SlowTorqueMult;
                LSlowMaxAddForce = SlowMaxAddForce;
                LSlowMaxAddTorque = SlowMaxAddTorque;
            }
        } else {
            LAirMaxAddForce = AirMaxAddForce;
            LAirMaxAddTorque = AirMaxAddTorque;
            LAirForceMult = AirForceMult;
            LAirTorqueMult = AirTorqueMult;
        }

        float MoveForce = DeltaTime *  SpeedForce * Body->GetMass() * LBoostForceMult * LSlowForceMult * LAirForceMult;
        Body->AddForce(Direction * MoveForce);

        FVector Velocity = Owner->GetVelocity();
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Velocity %f"),Velocity.Size()));
        if(Velocity.Z < 0.0f){
            Velocity.Z = 0.0f;
        }
        if(Velocity.Size() > MaximumVelocityForce + LBoostMaxAddForce + LSlowMaxAddForce + LAirMaxAddForce){
            Velocity.Z = 0.0f;
            FVector OppositeVelocityDirection = -1 * 
                UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                        Velocity);
            Body->AddForce(OppositeVelocityDirection * MoveForce);
        }


        //Rotational torque for movement
        FVector RotationDirection;
        RotationDirection.Y = Direction.X;
        RotationDirection.X = Direction.Y * (-1);
        RotationDirection.Z = Direction.Z;
        float MoveTorque = DeltaTime * SpeedTorque * Body->GetMass() * LBoostTorqueMult * LSlowTorqueMult * LAirTorqueMult;
        FVector AngularVelocity = Body->GetPhysicsAngularVelocityInDegrees();
        if(AngularVelocity.Size() > MaximumVelocityTorque + LBoostMaxAddTorque + LSlowMaxAddTorque + LAirMaxAddTorque){
            FVector UnitAngularDirection = 
                UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                        AngularVelocity);
            FVector OppositeVelocityRotation = -1 * UnitAngularDirection;
            Body->AddTorqueInDegrees(OppositeVelocityRotation * MoveTorque );
        }
    } else if(Grounded){
        FVector Griped = -1 * Owner->GetVelocity() * GripForce;
        Body->AddForce(DeltaTime * Griped * Body->GetMass());
        FVector GripedAngular = -1 * (Body->GetPhysicsAngularVelocityInDegrees()) * GripTorque;
        Body->AddTorqueInDegrees(DeltaTime * GripedAngular * Body->GetMass());
    }


    Body->AddForce(FVector(0.0f,0.0f,-1.0f) * ExtraGravity);
}


// Called every frame
void UMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

