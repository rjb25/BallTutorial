// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplicateMarker.generated.h"

class AModularPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALLTUTORIAL_API UReplicateMarker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReplicateMarker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    AActor * Marker = nullptr;
    APawn * MarkerPawn = nullptr;
    UStaticMeshComponent * MarkerBody = nullptr;
    AModularPlayerController * Controller = nullptr;
		
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerSetPosition(FTransform position);
	bool ServerSetPosition_Validate(FTransform position);
	void ServerSetPosition_Implementation(FTransform position);

	UFUNCTION( NetMulticast, Unreliable,  WithValidation)
	void ClientSetPosition(FTransform position);
    bool ClientSetPosition_Validate(FTransform position);
    void ClientSetPosition_Implementation(FTransform position);
};
