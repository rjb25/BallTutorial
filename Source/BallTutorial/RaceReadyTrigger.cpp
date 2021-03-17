// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceReadyTrigger.h"
#include "RacingPlayerState.h"
#include "RacingGameState.h"
#include "RollingBall.h"
#include "Components/BoxComponent.h"

ARaceReadyTrigger::ARaceReadyTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	UBoxComponent * OurTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OurTriggerBox"));
	OurTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	OurTriggerBox->SetupAttachment(RootComponent);
	OurTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARaceReadyTrigger::OnOverlapBegin);
	OurTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARaceReadyTrigger::OnOverlapEnd);

}

void ARaceReadyTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaceReadyTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaceReadyTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	ARollingBall * pawn = Cast<ARollingBall>(OtherActor);
	if (pawn && (OtherActor != this) && OtherComp) {
		ARacingPlayerState * playerState = Cast<ARacingPlayerState>(pawn->GetPlayerState());
		playerState->serverSetPlayerReady(true);
	}
}

void ARaceReadyTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}
