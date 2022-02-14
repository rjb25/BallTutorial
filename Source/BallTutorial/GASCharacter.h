// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "GASCharacter.generated.h"

UCLASS()
class BALLTUTORIAL_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
    class UGASAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
    class UGASAttributeSet* Attributes;

	// Sets default values for this character's properties
	AGASCharacter();

    virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual void InitializeAttributes();
    virtual void GiveAbilities();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
    TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
