// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/KismetMathLibrary.h"
#include "Drag.h"

// Sets default values for this component's properties
UDrag::UDrag()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDrag::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();
        //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                //FString::Printf(TEXT("Torque %f"), FlatDragTorque));

	// ...
	
}


// Called every frame
void UDrag::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
        //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                //FString::Printf(TEXT("TICK")));
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    FVector Velocity = Owner->GetVelocity();
    if(Velocity.Size() > 10.0f){
    FVector UnitDirection = 
        UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                Velocity);
    FVector Drag = -1 * UnitDirection * (Velocity * Velocity) * DragForce;
    Body->AddForce(DeltaTime * Drag * Body->GetMass());
        FVector FlatDrag = -1 * UnitDirection * FlatDragForce;
        Body->AddForce(DeltaTime * FlatDrag * Body->GetMass());
        //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                //FString::Printf(TEXT("Force %s %f"), *(DeltaTime * FlatDrag * Body->GetMass()).ToString(), FlatDragForce));
    }

    FVector AngularVelocity = Body->GetPhysicsAngularVelocityInDegrees();
    if(AngularVelocity.Size() > 10.0f){
    FVector UnitAngularDirection = 
        UKismetMathLibrary::GetDirectionUnitVector(FVector(0.0f,0.0f,0.0f), 
                AngularVelocity);
    FVector DragAngular = -1 * UnitAngularDirection * (AngularVelocity * AngularVelocity) * DragTorque;
    Body->AddTorqueInDegrees(DeltaTime * DragAngular * Body->GetMass());
        FVector FlatDragAngular = -1 * UnitAngularDirection * FlatDragTorque;
        Body->AddTorqueInDegrees(DeltaTime * FlatDragAngular * Body->GetMass());
        /*
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                FString::Printf(TEXT("Torque %s %f"), *(DeltaTime * FlatDragAngular * Body->GetMass()).ToString(), FlatDragTorque));
                */
    }
}
