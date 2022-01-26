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
    TArray<UStaticMeshComponent*> Bodies;
    Owner->GetComponents<UStaticMeshComponent>(Bodies);
    for(UStaticMeshComponent * Bod : Bodies){
        if (Bod->GetMass() > 0.001){
            Body = Bod;
            break;
        }
    }

    if (Body != nullptr){
        Body->SetSimulatePhysics(true);
        Body->SetNotifyRigidBodyCollision(true);
        Body->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
    } else {
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CHICKEN BEFORE THE EGG IN MOVE.cpp")));
    }
    //if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
            //FString::Printf(TEXT("SpeedForce %f SpeedTorque %f"),SpeedForce, SpeedTorque));
}

void UMove::Move(FVector Direction, float DeltaTime, bool Boost, bool Slow){
    bool Grounded = MyUtil::GroundCheck(Owner,GripDepth,GripWidth);
    /*
        if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                FString::Printf(TEXT("Force %s"), *(Direction).ToString()));
                */


    bool ActiveMovement = !Direction.IsZero();
    if(Body != nullptr){
        float Mass = Body->GetMass() + 0.1f;
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

        if (!Grounded){
            LAirMaxAddForce = AirMaxAddForce;
            LAirMaxAddTorque = AirMaxAddTorque;
            LAirForceMult = AirForceMult;
            LAirTorqueMult = AirTorqueMult;
        }

        float MoveForce = DeltaTime *  SpeedForce * Mass * LBoostForceMult * LSlowForceMult * LAirForceMult;
        Body->AddForce(Direction * MoveForce);

        FVector Velocity = Owner->GetVelocity();
        Velocity.Z = 0.0f;
        float VelocitySize = Velocity.Size();
        float TrueMaxSpeed = MaximumVelocityForce + LBoostMaxAddForce + LSlowMaxAddForce + LAirMaxAddForce;
        float GripMoveForce = MoveForce * FMath::Min(VelocitySize/TrueMaxSpeed, 1.0f);


        FVector OppositeVelocityDirection = -1 * 
            UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                    Velocity);

        //Rotational torque for movement
        FVector RotationDirection;
        RotationDirection.Y = Direction.X;
        RotationDirection.X = Direction.Y * (-1);
        RotationDirection.Z = Direction.Z;

        float MoveTorque = DeltaTime * SpeedTorque * Mass * LBoostTorqueMult * LSlowTorqueMult * LAirTorqueMult;
        Body->AddTorqueInDegrees(RotationDirection * MoveTorque);

        FVector AngularVelocity = Body->GetPhysicsAngularVelocityInDegrees();
        float AngularVelocitySize = AngularVelocity.Size();
        float TrueMaxRotation = MaximumVelocityTorque + LBoostMaxAddTorque + LSlowMaxAddTorque + LAirMaxAddTorque;
        float GripMoveTorque = MoveTorque * FMath::Min(AngularVelocitySize/TrueMaxRotation, 1.0f);
        FVector UnitAngularDirection = 
            UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                    AngularVelocity);

        FVector OppositeVelocityRotation = -1 * UnitAngularDirection;
        FVector AntiTwirlRotation = FVector(0.0f, 0.0f, -1 * FMath::Sign(UnitAngularDirection.Z));

        if(ActiveMovement){
            if(VelocitySize > TrueMaxSpeed){
                Body->AddForce(OppositeVelocityDirection * MoveForce);
                if(!Boost) Body->AddForce(OppositeVelocityDirection * GripMoveForce);
            }

            if(AngularVelocitySize > TrueMaxRotation){
                Body->AddTorqueInDegrees(OppositeVelocityRotation * MoveTorque );
                if(!Boost) Body->AddTorqueInDegrees(OppositeVelocityRotation * GripMoveTorque );
            }
        } else {
            if(VelocitySize > 0.001f){
                Body->AddForce(OppositeVelocityDirection * GripMoveForce);
            }
            if(AngularVelocitySize > 0.1f){
                Body->AddTorqueInDegrees(OppositeVelocityRotation * GripMoveTorque);
            }
        }


        Body->AddTorqueInDegrees(AntiTwirlRotation * AntiTwirlGrip );
        Body->AddForce(FVector(0.0f,0.0f,-1.0f) * ExtraGravity);
    }
}

/*
void UMove::Grip(float DeltaTime){
    GripVelocity(DeltaTime);
    GripAngularVelocity(DeltaTime);
}
void UMove::GripVelocity(float DeltaTime){
    FVector Velocity = Owner->GetVelocity();
    Velocity.Z = 0.0f;
    FVector Gripped = -1 * Velocity * GripForce;
    Body->AddForce(DeltaTime * Gripped * Body->GetMass());
}

void UMove::GripAngularVelocity(float DeltaTime){
    FVector AngularVelocity = Body->GetPhysicsAngularVelocityInDegrees();
    FVector GrippedAngular = -1 * AngularVelocity * GripTorque;
    Body->AddTorqueInDegrees(DeltaTime * GrippedAngular * Body->GetMass());
}
*/


// Called every frame
void UMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

