// Fill out your copyright notice in the Description page of Project Settings.


#include "Jump.h"
#include "MyUtil.h"

// Sets default values for this component's properties
UJump::UJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    JumpForce = 1000.0f;
    GripWidth = 40.0f;
    GripDepth = 40.0f;
    Body = nullptr;
    Owner = nullptr;
    JumpReload = 0.2f;
    LastJump = 0.0f;

	// ...
}


// Called when the game starts
void UJump::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();

	// ...
	
}


// Called every frame
void UJump::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJump::Jump(){
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if(MyUtil::GroundCheck(Owner,GripDepth,GripWidth) && LastJump + JumpReload < CurrentTime){
        Body->AddImpulse(FVector(0.0f,0.0f,1.0f) * JumpForce * Body->GetMass());
        LastJump = CurrentTime;
    }
}
