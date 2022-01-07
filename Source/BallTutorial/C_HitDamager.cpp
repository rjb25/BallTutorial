// Fill out your copyright notice in the Description page of Project Settings.

#include "C_HitDamager.h"
#include "RollingBall.h"

#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UC_HitDamager::UC_HitDamager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    m_prevHitTime = -10;
    m_damage = 1.0f;
    m_selfFire = false;

	// ...
}

// Called when the game starts
void UC_HitDamager::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	Owner->OnActorHit.AddDynamic(this, &UC_HitDamager::DealDamage);
    
    Owner->FindComponentByClass<UStaticMeshComponent>()->SetNotifyRigidBodyCollision(true);
	
}

// Called every frame
void UC_HitDamager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UC_HitDamager::DealDamage(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit){
    float currentTime = GetWorld()->GetTimeSeconds();
    if( currentTime > 1.0f + m_prevHitTime){
        UHealthComponent * health = OtherActor->FindComponentByClass<UHealthComponent>();

        ARollingBall * pawn = Cast<ARollingBall>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
        if(health != nullptr && (m_selfFire || OtherActor != Owner)){
            if(health->m_networked){
                pawn->ServerHurt(OtherActor, m_damage);
            }else{
                health->Suffer(m_damage);
            }
            m_prevHitTime = currentTime;
        }
    }
}
