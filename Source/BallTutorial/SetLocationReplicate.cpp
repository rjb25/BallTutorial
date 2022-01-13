// Fill out your copyright notice in the Description page of Project Settings.


#include "SetLocationReplicate.h"

// Sets default values for this component's properties
USetLocationReplicate::USetLocationReplicate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(true);

	// ...
}


// Called when the game starts
void USetLocationReplicate::BeginPlay()
{
	Super::BeginPlay();
    
    Owner = GetOwner();
    Pawn = Cast<APawn>(Owner);
    Body = Owner->FindComponentByClass<UStaticMeshComponent>();

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("I spawned: %s %s"), *Pawn->GetName(), Pawn->HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT")));

    if (!(Pawn->IsLocallyControlled())) {
        Body->SetSimulatePhysics(false);
        Body->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    } 
}


// Called every frame
void USetLocationReplicate::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (Pawn->HasAuthority()) {
        ClientSetPosition(Pawn->GetActorTransform());
    }
    //Send local positions to server which will then send our position to other clients
    else if (!(Pawn->HasAuthority()) && Pawn->IsLocallyControlled()) {
        ServerSetPosition(Owner->GetActorTransform());
    }
}

bool USetLocationReplicate::ServerSetPosition_Validate(FTransform position) {
    return true;
}

void USetLocationReplicate::ServerSetPosition_Implementation(FTransform position) {
    Owner->SetActorTransform(position);
}

bool USetLocationReplicate::ClientSetPosition_Validate(FTransform position) {
    return true;
}

void USetLocationReplicate::ClientSetPosition_Implementation(FTransform position) {
    if (!(Pawn->HasAuthority()) &&  !(Pawn->IsLocallyControlled())    ) {
        Owner->SetActorTransform(position);
    }
}
