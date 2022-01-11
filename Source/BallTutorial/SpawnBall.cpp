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
    m_speed = 2000.0f;
    m_reload = 3.0f;
    m_timeLastFire = 0.0f;
}

// Called when the game starts
void USpawnBall::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
    AI = Owner->FindComponentByClass<UEnemyAIBasic>();
}

void USpawnBall::Spawn(FVector Direction)
{
    if(ActorToSpawn != nullptr){
        float time = GetWorld()->GetTimeSeconds();
        if (time > m_timeLastFire + m_reload){
            AActor * owner = GetOwner();
            FVector SpawnLocation = owner->GetActorLocation() + FVector(0.0,0.0,200.0);
            FRotator SpawnRotation = owner->GetActorRotation();
            AActor* TempProjectile = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
            UStaticMeshComponent * body = TempProjectile->FindComponentByClass<UStaticMeshComponent>();
            body->AddImpulse(Direction * m_speed * body->GetMass());
            m_timeLastFire = GetWorld()->GetTimeSeconds();
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }
}

void USpawnBall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (AI != nullptr && AI->Enabled && AI->Targets.Num() > 0){
        FVector TowardTarget = UKismetMathLibrary::GetDirectionUnitVector(Owner->GetActorLocation(), AI->Targets[0]->GetActorLocation());
        Spawn(TowardTarget);
    }
}
