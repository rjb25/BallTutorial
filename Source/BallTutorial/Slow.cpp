// Fill out your copyright notice in the Description page of Project Settings.


#include "Slow.h"
#include "MyUtil.h"

// Sets default values for this component's properties
USlow::USlow()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USlow::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();

	// ...
	
}


// Called every frame
void USlow::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USlow::Slow(float DeltaTime){
    bool Grounded = MyUtil::GroundCheck(Owner,GripDepth,GripWidth);
    if(Grounded){
        FVector Slowed = -1 * Owner->GetVelocity() * SlowForce;
        Body->AddForce(DeltaTime * Slowed * Body->GetMass());
        FVector SlowedAngular = -1 * (Body->GetPhysicsAngularVelocityInDegrees()) * SlowTorque;
        Body->AddTorqueInDegrees(DeltaTime * SlowedAngular * Body->GetMass());
    }
}
