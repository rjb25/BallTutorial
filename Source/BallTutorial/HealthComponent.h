// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ARacingGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 3.0f;
	float Health = 0.0f;
	ARacingGameMode* GameModeRef;
	AActor* Owner;

public:	
	// Sets default values for this component's properties
	UHealthComponent();
    UFUNCTION()
    void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//void Suffer(float Damage);

	//void Hurt(float Damage);
	void Death();

    /*
	UFUNCTION( NetMulticast, Reliable,  WithValidation)
	void ClientSuffer(float Damage);
    bool ClientSuffer_Validate(float Damage);
    void ClientSuffer_Implementation(float Damage);
    */
    UPROPERTY()
    TArray<int> Teams;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
