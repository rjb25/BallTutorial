// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIBasic.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "RollingBall.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UEnemyAIBasic::UEnemyAIBasic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    Speed = 1000.0f;
    SensoryScale = FVector(1.0f, 1.0f, 1.0f);
    Direction = FVector(0.0f, 0.0f, 0.0f);
    Enabled = true;
}

// Called when the game starts
void UEnemyAIBasic::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    StartLocation = Owner->GetActorLocation();
    if (SensoryBox == nullptr){
        if (SensoryBoxToSpawn == nullptr){
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("No senses on an AI")));
        }else{
            FVector SpawnLocation = Owner->GetActorLocation();
            FRotator SpawnRotation = Owner->GetActorRotation();
            SensoryBox = GetWorld()->SpawnActor<AActor>(SensoryBoxToSpawn,SpawnLocation, SpawnRotation);
            SensoryBox->SetActorRelativeScale3D(SensoryScale);
            Cast<AActor>(SensoryBox)->OnActorBeginOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapBegin);
            Cast<AActor>(SensoryBox)->OnActorEndOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapEnd);
        }
    } else {
        Cast<AActor>(SensoryBox)->OnActorBeginOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapBegin);
        Cast<AActor>(SensoryBox)->OnActorEndOverlap.AddDynamic(this, &UEnemyAIBasic::OnOverlapEnd);

    }
}


// Called every frame
void UEnemyAIBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnemyAIBasic::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor){
	ARollingBall * pawn = Cast<ARollingBall>(OtherActor);
	if (pawn != nullptr) {
        Targets.AddUnique(OtherActor);
        OldestTarget = Targets[0];
	}
}

void UEnemyAIBasic::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor) {
	ARollingBall * pawn = Cast<ARollingBall>(OtherActor);
	if (pawn != nullptr) {
        Targets.Remove(OtherActor);
        if (Targets.Num() > 0) {
            OldestTarget = Targets[0];
        }
    }
}
