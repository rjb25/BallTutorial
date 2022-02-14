 // Fill out your copyright notice in the Description page of Project Settings.


#include "ContactDamage.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "ModularPlayerController.h"

UContactDamage::UContactDamage()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UContactDamage::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
    if(Real){
        Owner->OnActorHit.AddDynamic(this, &UContactDamage::ContactHit);
        Owner->OnActorBeginOverlap.AddDynamic(this, &UContactDamage::ContactOverlap);
    }
}

void UContactDamage::ContactHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit){
    DealDamage(SelfActor,OtherActor);
}

void UContactDamage::ContactOverlap(AActor* OverlappedActor, AActor* OtherActor){
    DealDamage(OverlappedActor,OtherActor);
}

void UContactDamage::DealDamage(AActor* Sender, AActor* Receiver){
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if( CurrentTime > HitRate + TimeLastHit){
        UHealthComponent * Health = Receiver->FindComponentByClass<UHealthComponent>();
        APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        AModularPlayerController * MyController = Cast<AModularPlayerController>(PController);
        if (Health != nullptr) {
            bool Hurt = true;
            for (int Team : Teams){
                if(Health->Teams.Contains(Team)){
                    Hurt = false;
                    break;
                }
            }
            if(Hurt){
                if (!Owner->HasAuthority() && MyController != nullptr) {
                    MyController->ServerHurt(Receiver, Damage, DamageType);
                }
                else {
                    UGameplayStatics::ApplyDamage(Receiver, Damage, PController, Owner, DamageType);
                }
                TimeLastHit = CurrentTime;
            }
        }
    }
}
