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
    PrimaryComponentTick.bCanEverTick = false;
    m_speed = 2000.0f;
    m_reload = 3.0f;
    m_timeLastFire = 0.0f;

        // ...
}

// Called when the game starts
void USpawnBall::BeginPlay()
{
    Super::BeginPlay();
    ICanActInterface * owner = Cast<ICanActInterface>(GetOwner());
    if (owner != nullptr) {
        owner->setAct(Cast<IActableInterface>(this));
    }
}

void USpawnBall::act()
{
    float time = GetWorld()->GetTimeSeconds();
    if (time > m_timeLastFire + m_reload){
        AActor * owner = GetOwner();
        FVector SpawnLocation = owner->GetActorLocation() + FVector(0.0,0.0,200.0);
        FRotator SpawnRotation = owner->GetActorRotation();
        AActor* TempProjectile = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
        UStaticMeshComponent * body = TempProjectile->FindComponentByClass<UStaticMeshComponent>();

        USpringArmComponent * spring = owner->FindComponentByClass<USpringArmComponent>();
        FRotator springRotation = spring->GetComponentRotation();
        FVector verticalAxis = { 0,0,1 };
        FVector rotatedUnitDirection = UKismetMathLibrary::RotateAngleAxis(FVector(1.0f,0.0f,0.0f), springRotation.Yaw, verticalAxis);

        body->AddImpulse(rotatedUnitDirection * m_speed * body->GetMass());
        m_timeLastFire = GetWorld()->GetTimeSeconds();
    }
}

