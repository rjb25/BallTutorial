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
    //if(Owner->controller = world->controller) then we know to do this. Otherwise check authority
    //Will need one for multicast to spawn bots ammo and server players ammo also to spawn replicas of client side bullets
    //client side bullets will know to spawn on the client because bullets will have a reference to their shooter? so if local player controller == my shooter player controller don't spawn?
    if(ActorToSpawn != nullptr){
        float time = GetWorld()->GetTimeSeconds();
        if (time > TimeLastFire + Reload){
            if(Owner->HasAuthority()){
                FRotator OffsetRotator = UKismetMathLibrary::MakeRotFromX(Direction);
                FVector SpawnLocation = Owner->GetActorLocation() + OffsetRotator.RotateVector(ProjectileOffset);
                AActor* TempProjectile = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, ProjectileRotation);
                AActor * PreProjectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ActorToSpawn, FTransform(ProjectileRotation, SpawnLocation, FVector(1.0f,1.0f,1.0f)));
                if(PreProjectile != nullptr){
                    UContactDamage * DamageComp = PreProjectile->FindComponentByClass<UContactDamage>();
                    DamageComp->Real = true;
                    if(DamageComp != nullptr && HealthComp != nullptr){
                        for(int Team : HealthComp->Teams){
                            DamageComp->Teams.Add(Team);
                        }

                    }
                }
                UGameplayStatics::FinishSpawningActor(PreProjectile,FTransform(ProjectileRotation, SpawnLocation, FVector(1.0f,1.0f,1.0f)));

                TimeLastFire = GetWorld()->GetTimeSeconds();
            }
        }
    } else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor class to spawn not set")));

    }
}

/*
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
*/
