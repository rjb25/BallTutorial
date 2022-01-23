// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "Soul.h"
#include "EnemyAIBasic.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerCenter"));
    RootComponent = mesh;
    SetActorHiddenInGame(true);

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

    if(SensorBox != nullptr){
    SensorBox->OnActorBeginOverlap.AddDynamic(this, &AWaveManager::OnOverlapBegin);
    SensorBox->OnActorEndOverlap.AddDynamic(this, &AWaveManager::OnOverlapEnd);
    }
    for ( int WaveIndex = 0; WaveIndex < Waves.Num(); WaveIndex++){
        FWave * Wave = &Waves[WaveIndex];
        for (int SwitchIndex = 0; SwitchIndex < Wave->Switches.Num(); SwitchIndex++) {
            FSwitchable * Switch = &Wave->Switches[SwitchIndex];
            Switch->HiddenDefault = Switch->ActorToManipulate->IsHidden();
            Switch->CollisionDefault = Switch->ActorToManipulate->GetActorEnableCollision();
            Switch->TickDefault = Switch->ActorToManipulate->IsActorTickEnabled();
            if(Switch->DefaultShow){
                Show(Switch);
            } else {
                Hide(Switch);
            }
        }
    }
}

void AWaveManager::Show(FSwitchable * Switch){
    Switch->ActorToManipulate->SetActorHiddenInGame(Switch->HiddenDefault);
    Switch->ActorToManipulate->SetActorEnableCollision(Switch->CollisionDefault);
    Switch->ActorToManipulate->SetActorTickEnabled(Switch->CollisionDefault);
}

void AWaveManager::Hide(FSwitchable * Switch){
    Switch->ActorToManipulate->SetActorHiddenInGame(true);
    Switch->ActorToManipulate->SetActorEnableCollision(false);
    Switch->ActorToManipulate->SetActorTickEnabled(false);
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveManager::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor){
    ASoul * OtherSoul = Cast<ASoul>(OtherActor);
    if(OtherSoul != nullptr){
        if(OtherSoul->PlayerController != nullptr){
            SpawnWave();
        }
    }
}

void AWaveManager::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor){

}

void AWaveManager::SpawnWave(){
    if(CurrentWave < Waves.Num()){
        for(FSpawnable Spawn : Waves[CurrentWave].Spawns){
            if(Spawn.SoulToSpawn != nullptr){
                SpawnIt(Spawn);
            }
        }
        for(FSwitchable Switch : Waves[CurrentWave].Switches){
            if(Switch.ActorToManipulate != nullptr){
                if(Switch.Visible){
                    Show(&Switch);
                } else {
                    Hide(&Switch);
                }
            }
        }

           /*
           for(FDropable Drop : Waves[CurrentWave].Drops){
           DropIt(Drop);
           }
           */
        CurrentWave++;
    }
}

void AWaveManager::SpawnIt(FSpawnable Spawn){
    FVector SpawnLocation;
    if(SpawnLocations.Num() > 0){
        SpawnLocation = GetActorLocation()+SpawnLocations[Spawn.LocationIndex];
    }else{
        SpawnLocation = GetActorLocation();
    }
    FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Spawn.SoulToSpawn, SpawnLocation, SpawnRotation);
    if(SpawnedActor){
        SpawnedActor->OnDestroyed.AddDynamic(this, &AWaveManager::SoulDied);
        UEnemyAIBasic * EnemyAIBasic = SpawnedActor->FindComponentByClass<UEnemyAIBasic>();
        EnemyAIBasic->SetSensorBox(SensorBox);
    }
    if(Spawn.MustDie){
        ActorsAlive++;
    }
}

/*

void AWaveManager::DropIt(FDropable Drop){
}
*/
void AWaveManager::SoulDied(AActor * DestroyedActor){
    ActorsAlive--;
    if (ActorsAlive < 1) {
        SpawnWave();
    }
}
