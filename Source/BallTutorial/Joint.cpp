// Fill out your copyright notice in the Description page of Project Settings.


#include "Joint.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UJoint::UJoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
    Owner = GetOwner();
}


// Called when the game starts
void UJoint::BeginPlay()
{
	Super::BeginPlay();
    for (int JoinableIndex = 0; JoinableIndex < Joinables.Num(); JoinableIndex++){
        FJoinable * Joinable = &Joinables[JoinableIndex];
        FVector SpawnLocation = Owner->GetActorLocation() + Joinable->Offset;
        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Joinable->ActorToSpawn, SpawnLocation, Joinable->Rotation);
        Joinable->Actor = SpawnedActor;
        Owner->OnDestroyed.AddDynamic(this, &UJoint::KillActor);
    }
}


// Called every frame
void UJoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int JoinableIndex = 0; JoinableIndex < Joinables.Num(); JoinableIndex++){
        FJoinable * Joinable = &Joinables[JoinableIndex];
        FVector VerticalAxis = { 0,0,1 };
        FVector RotatedOffset = 
            UKismetMathLibrary::RotateAngleAxis(Joinable->Offset, 
                    CurrentRotation.Yaw, 
                    VerticalAxis);
        FVector CurrentLocation = Owner->GetActorLocation() + RotatedOffset;

        Joinable->Actor->SetActorLocation(CurrentLocation);
        //Current rotation set from either ai  or playercontroller
        FRotator MyCurrentRotation = FRotator(0.0f, CurrentRotation.Yaw, 0.0f );
        Joinable->Actor->SetActorRotation(MyCurrentRotation);
    }
}

void UJoint::KillActor(AActor * ActorThatDied){
    for (int JoinableIndex = 0; JoinableIndex < Joinables.Num(); JoinableIndex++){
        FJoinable * Joinable = &Joinables[JoinableIndex];
        Joinable->Actor->Destroy(true);
    }
}
