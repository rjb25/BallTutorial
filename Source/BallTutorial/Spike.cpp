// Fill out your copyright notice in the Description page of Project Settings.

#include "Spike.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USpike::USpike()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpike::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	Owner->OnActorBeginOverlap.AddDynamic(this, &USpike::DealDamage);

	
}


// Called every frame
void USpike::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USpike::DealDamage(AActor* OverlappedActor, AActor* OtherActor){
	TSubclassOf<UDamageType> P;
	UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, Owner, P);
}
