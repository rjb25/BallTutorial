// Fill out your copyright notice in the Description page of Project Settings.


#include "Jump.h"
#include "MyUtil.h"

// Sets default values for this component's properties
UJump::UJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UJump::BeginPlay()
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
}


// Called every frame
void UJump::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJump::Jump(){
    if(Body!=nullptr){
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if(MyUtil::GroundCheck(Owner,GripDepth,GripWidth) && LastJump + JumpReload < CurrentTime){
            FVector Velocity = Owner->GetVelocity();
            UPrimitiveComponent * Primitive = Cast<UPrimitiveComponent>(Body);
            if (Primitive != nullptr){
                Primitive->SetPhysicsLinearVelocity(FVector(Velocity.X,Velocity.Y,0.0f));
            }
            Body->AddImpulse(FVector(0.0f,0.0f,1.0f) * JumpForce * Body->GetMass());
            LastJump = CurrentTime;
        }
    }
}
