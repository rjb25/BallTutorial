// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnBall.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CanActInterface.h"
#include "HealthComponent.h"
#include "ContactDamage.h"

// Sets default values for this component's properties
USpawnBall::USpawnBall()
{
}

// Called when the game starts
void USpawnBall::BeginPlay()
{
    Super::BeginPlay();
    Owner = GetOwner();
    HealthComp = Owner->FindComponentByClass<UHealthComponent>();
}

void USpawnBall::Use(FVector Direction)
{
    ServerSpawn(Direction);
}

bool USpawnBall::ServerSpawn_Validate(FVector Direction) {
    return true;
}

void USpawnBall::ServerSpawn_Implementation(FVector Direction) {
    if(ActorToSpawn != nullptr){
        float time = GetWorld()->GetTimeSeconds();
        if (time > TimeLastFire + Reload){
            FRotator OffsetRotator = UKismetMathLibrary::MakeRotFromX(Direction);
            FVector SpawnLocation = Owner->GetActorLocation() + OffsetRotator.RotateVector(ProjectileOffset);
            AActor * PreProjectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ActorToSpawn, FTransform(ProjectileRotation, SpawnLocation, FVector(1.0f,1.0f,1.0f)));
            if(PreProjectile != nullptr){
                UContactDamage * DamageComp = PreProjectile->FindComponentByClass<UContactDamage>();
                if(DamageComp != nullptr && HealthComp != nullptr){
                    DamageComp->Real = true;
                    for(int Team : HealthComp->Teams){
                        DamageComp->Teams.Add(Team);
                    }

                }
            }
            UGameplayStatics::FinishSpawningActor(PreProjectile,FTransform(ProjectileRotation, SpawnLocation, FVector(1.0f,1.0f,1.0f)));

            TimeLastFire = GetWorld()->GetTimeSeconds();
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }

}
