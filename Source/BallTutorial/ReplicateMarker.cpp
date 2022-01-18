// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicateMarker.h"
#include "Kismet/GameplayStatics.h"
#include "ModularPlayerController.h"
#include "Soul.h"

// Sets default values for this component's properties
UReplicateMarker::UReplicateMarker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReplicateMarker::BeginPlay()
{
	Super::BeginPlay();
    Marker = GetOwner();
    MarkerBody = Marker->FindComponentByClass<UStaticMeshComponent>();
    MarkerPawn = Cast<APawn>(Marker);

    APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    Controller = Cast<AModularPlayerController>(PController); 
    if(MarkerPawn->GetController() == PController){
        MarkerBody->ToggleVisibility(false);
    }
}


// Called every frame
void UReplicateMarker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Marker != nullptr){
        if (Marker->HasAuthority()) {
            ClientSetPosition(Controller->Soul->GetActorTransform());
        }
        //Send local positions to server which will then send our position to other clients
        else if (!(Marker->HasAuthority()) && MarkerPawn->IsLocallyControlled()) {
            ServerSetPosition(Controller->Soul->GetActorTransform());
        }
    }
}

bool UReplicateMarker::ServerSetPosition_Validate(FTransform position) {
    return true;
}

void UReplicateMarker::ServerSetPosition_Implementation(FTransform position) {
    Marker->SetActorTransform(position);
}

bool UReplicateMarker::ClientSetPosition_Validate(FTransform position) {
    return true;
}

void UReplicateMarker::ClientSetPosition_Implementation(FTransform position) {
    if (!(Marker->HasAuthority()) && !(MarkerPawn->IsLocallyControlled())) {
        Marker->SetActorTransform(position);
    }
}
