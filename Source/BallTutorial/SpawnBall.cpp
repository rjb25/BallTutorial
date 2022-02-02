// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnBall.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "CanActInterface.h"
#include "HealthComponent.h"
#include "ContactDamage.h"

// Sets default values for this component's properties
USpawnBall::USpawnBall()
{
    ProjectileSpeed = 2000.0f;
    Reload = 3.0f;
    TimeLastFire = 0.0f;
}

// Called when the game starts
void USpawnBall::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
    HealthComp = Owner->FindComponentByClass<UHealthComponent>();
    //Owner->OnDestroyed.AddDynamic(this, &AActor::Destroy);
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
            UStaticMeshComponent * Body = TempProjectile->FindComponentByClass<UStaticMeshComponent>();
            UContactDamage * DamageComp = TempProjectile->FindComponentByClass<UContactDamage>();
            if(DamageComp != nullptr && HealthComp != nullptr){
                for(int Team : HealthComp->Teams){
                    DamageComp->Teams.Add(Team);
                }

            }
            if (Body != nullptr && Body->IsSimulatingPhysics()){
                Body->AddImpulse(Direction * ProjectileSpeed * Body->GetMass());
            }
            TimeLastFire = GetWorld()->GetTimeSeconds();
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }
}
