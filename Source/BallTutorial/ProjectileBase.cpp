// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.0f;
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit made!"));
	}
	// Try to get a reference to the owning class.
	AActor* MyOwner = GetOwner();
	// If for some reason we can't get a valid reference, return as we need to check against the owner. 
	if(!MyOwner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Owner not found"));
		return;
	}
	// If the other actor ISN'T self OR Owner AND exists, then apply damage.
	if(OtherActor && OtherActor != this /*&& OtherActor != MyOwner*/)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		Destroy();
	}	
}
