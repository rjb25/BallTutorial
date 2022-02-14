// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ContactDamage.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UContactDamage : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UContactDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
    void ContactHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
    void ContactOverlap(AActor* OverlappedActor, AActor* OtherActor);

    void DealDamage(AActor* Sender, AActor* Receiver);

    float TimeLastHit = -10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere)
    float HitRate = 1.0f;
    UPROPERTY(EditAnywhere)
    float Damage = 1.0f;
    UPROPERTY(EditAnywhere)
    TArray<int> Teams;
    AActor * Owner = nullptr;
    bool Real = false;
};
