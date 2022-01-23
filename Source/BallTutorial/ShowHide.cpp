// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowHide.h"

// Sets default values for this component's properties
UShowHide::UShowHide()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShowHide::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();

    HiddenDefault = Owner->IsHidden();
    CollisionDefault = Owner->GetActorEnableCollision();
    TickDefault = Owner->IsActorTickEnabled();
}


// Called every frame
void UShowHide::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShowHide::Show(){
    Owner->SetActorHiddenInGame(HiddenDefault);
    Owner->SetActorEnableCollision(CollisionDefault);
	Owner->SetActorTickEnabled(TickDefault);
}

void UShowHide::Hide(){
    Owner->SetActorHiddenInGame(true);
    Owner->SetActorEnableCollision(false);
	Owner->SetActorTickEnabled(false);
}

