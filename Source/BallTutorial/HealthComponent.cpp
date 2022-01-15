// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RacingGameMode.h"
#include "RollingBall.h"
#include "ModularPlayerController.h"

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
    if(m_networked){
        owner->SetReplicates(true);
    }
}

void UHealthComponent::Suffer(float Damage){
    if(Damage == 0 || Health <= 0)
    {
        return;
    }

    //Only done locally don't worry about networking
    if (!m_networked) {
        Hurt(Damage);
    } 
    //Case for if this is networked and called on the server -> controls damage keeping
    else if (owner->HasAuthority()) { 
        ClientSuffer(Damage);
    } 
}

void UHealthComponent::Hurt(float Damage){
    Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Damage taken! Health: %f"), Health));

    if(Health <= 0)
    {
        APawn * Pawn = Cast<APawn>(owner);    

        if(Pawn != nullptr && Pawn->IsPlayerControlled())
        {
            AController * Controller = Pawn->GetController();
            if(Controller){
                AModularPlayerController * Player = Cast<AModularPlayerController>(Controller);
                if (Player){
                    Player->ToCheckpoint();
                }
            }
        }
        else 
        {
            Death();
        }    
        Health = DefaultHealth;
    }
}

void UHealthComponent::Death(){
    owner->Destroy(true);
}

bool UHealthComponent::ClientSuffer_Validate(float Damage) {
    return true;
}
void UHealthComponent::ClientSuffer_Implementation(float Damage) {
    Hurt(Damage);
}
