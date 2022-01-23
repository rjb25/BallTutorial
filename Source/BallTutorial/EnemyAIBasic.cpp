// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIBasic.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "RollingBall.h"
#include "Components/SphereComponent.h"
#include "Move.h"
#include "Jump.h"
#include "SpawnBall.h"
#include "Soul.h"

// Sets default values for this component's properties
UEnemyAIBasic::UEnemyAIBasic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEnemyAIBasic::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    StartLocation = Owner->GetActorLocation();
    if (SensorBox == nullptr){
        if (SensorBoxToSpawn == nullptr){
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("No senses on an AI")));
        }else{
            FVector SpawnLocation = Owner->GetActorLocation();
            FRotator SpawnRotation = Owner->GetActorRotation();
            SensorBox = GetWorld()->SpawnActor<AActor>(SensorBoxToSpawn,SpawnLocation, SpawnRotation);
            SensorBox->SetActorRelativeScale3D(SensoryScale);
        }
    } 
    if (SensorBox != nullptr){
        SetSensorBox(SensorBox);
    }
    if(Owner != nullptr){
        MovementComp = Owner->FindComponentByClass<UMove>();
        JumpComp = Owner->FindComponentByClass<UJump>();
        SpawnBallComp = Owner->FindComponentByClass<USpawnBall>();
        Body = Owner->FindComponentByClass<UStaticMeshComponent>();
        Primitive = Cast<UPrimitiveComponent>(Body);
    }
}

void UEnemyAIBasic::SetSensorBox(AActor* SensorBox){
        SensorBox->OnActorBeginOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapBegin);
        SensorBox->OnActorEndOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapEnd);
        TArray<AActor*> InitialTargets;
        SensorBox->GetOverlappingActors(InitialTargets);
        for( AActor * InitialTarget : InitialTargets){
            OnOverlapBegin(Owner,InitialTarget);
        }
}

// Called every frame
void UEnemyAIBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (OldestTarget != nullptr){
        FVector TargetLocation = OldestTarget->GetActorLocation();
        FVector MyLocation = Owner->GetActorLocation();
        MyLocation.Z = 0.0f;
        TargetLocation.Z = 0.0f;
        FVector UnitDirection = 
            UKismetMathLibrary::GetDirectionUnitVector(MyLocation, 
                    TargetLocation);
        //GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, 
                //FString::Printf(TEXT("Unit Direction %s"), *(UnitDirection).ToString()));
        if (MovementComp != nullptr){
            if (DeltaTime < 0.3f){
                MovementComp->Move(UnitDirection,DeltaTime);
            }
        }
        if (SpawnBallComp != nullptr){
            SpawnBallComp->Spawn(UnitDirection);
        }
        if (JumpComp != nullptr && JumpEvade){
            JumpComp->Jump();
        }
    } else {
            //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("No target")));
    }
    if(Owner != nullptr && Owner->IsReplicatingMovement()){
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Movement is replicating")));

    }
}

void UEnemyAIBasic::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor){
	ASoul * soul = Cast<ASoul>(OtherActor);
    //At some point this would be a comparison of tags between what we target and what they have
	if (soul != nullptr && Owner != OtherActor && soul->PlayerController != nullptr) {
        Targets.AddUnique(OtherActor);
        OldestTarget = Targets[0];
	}
}

void UEnemyAIBasic::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor) {
	ASoul * soul = Cast<ASoul>(OtherActor);
	if (soul != nullptr && Owner != OtherActor) {
        Targets.Remove(OtherActor);
        if (Targets.Num() > 0) {
            OldestTarget = Targets[0];
        } else {
            OldestTarget = nullptr;
        }
    }
}
