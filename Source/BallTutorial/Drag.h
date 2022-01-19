// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Drag.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UDrag : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDrag();

    UPROPERTY(EditAnywhere)
    float DragForce = 1.0f;
    UPROPERTY(EditAnywhere)
    float DragTorque = 1.0f;

    UPROPERTY(EditAnywhere)
    float FlatDragForce = 1000.0f;
    UPROPERTY(EditAnywhere)
    float FlatDragTorque = 1000.0f;
    UPROPERTY(EditAnywhere, Category = Jason)
    bool jasonbool;
    UPROPERTY(EditAnywhere, Category = Jason, meta = (EditCondition = jasonbool))
    bool jason;
    bool jason2;

private:
    AActor * Owner;
    UStaticMeshComponent * Body;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
