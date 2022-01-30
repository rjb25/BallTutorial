// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnBall.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "CanActInterface.h"

// Sets default values for this component's properties
USpawnBall::USpawnBall()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    ProjectileSpeed = 2000.0f;
    Reload = 3.0f;
    TimeLastFire = 0.0f;
}

// Called when the game starts
void USpawnBall::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
}

void USpawnBall::Use(FVector Direction)
{
    if(ActorToSpawn != nullptr){
        float time = GetWorld()->GetTimeSeconds();
        if (time > TimeLastFire + Reload){
            AActor * owner = GetOwner();
            FRotator OffsetRotator = UKismetMathLibrary::MakeRotFromX(Direction);
            FVector SpawnLocation = owner->GetActorLocation() + OffsetRotator.RotateVector(ProjectileOffset);
            AActor* TempProjectile = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, ProjectileRotation);
            UStaticMeshComponent * body = TempProjectile->FindComponentByClass<UStaticMeshComponent>();
            if (body != nullptr && body->IsSimulatingPhysics()){
                body->AddImpulse(Direction * ProjectileSpeed * body->GetMass());
            }
            TimeLastFire = GetWorld()->GetTimeSeconds();
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }
}

void USpawnBall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
