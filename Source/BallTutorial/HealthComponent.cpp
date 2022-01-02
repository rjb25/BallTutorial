// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RacingGameMode.h"
#include "RollingBall.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = DefaultHealth;	
    GameModeRef = Cast<ARacingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    owner = GetOwner();
}

void UHealthComponent::Suffer(float Damage){
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health SUCCESS: ")));
    if(Damage == 0 || Health <= 0)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damage taken! Health: %f"), Health));

    if(Health <= 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("DEATH!"), Health));
        AActor * DeadActor = GetOwner();
        ARollingBall * player = Cast<ARollingBall>(DeadActor);    

        if(player)
        {
            player->timeout(1.0f);
            player->checkpoint();
        }
        else 
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SUPERDEATH!"), Health));
            DeadActor->Destroy(true);
            //owner->SetActorLocation(owner->GetActorLocation() + FVector(0.0f,0.0f,100.0f));
        }    
        Health = DefaultHealth;
    }
}
