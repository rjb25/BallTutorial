// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnBall.h"
#include "Camera/CameraComponent.h"
#include "BallTutorial/ProjectileBase.h"

// Sets default values for this component's properties
USpawnBall::USpawnBall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnBall::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();

	// ...

}
void USpawnBall::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Do", IE_Pressed, this, &USpawnBall::SpawnIt);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}


// Called every frame
void USpawnBall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpawnBall::SpawnIt()
{
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	FVector SpawnLocation = Camera->GetComponentLocation();
	FRotator SpawnRotation = Camera->GetComponentRotation();
	if(ProjectileClass)
	{
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation + offset, SpawnRotation);
		TempProjectile->SetOwner(GetOwner());
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Class set!"));
		}
	} else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Class not set!"));
		}
	}
}

