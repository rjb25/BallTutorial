// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceStepTrigger.h"
#include "RacingPlayerState.h"
#include "RacingGameState.h"
#include "RollingBall.h"
#include "Components/BoxComponent.h"

ARaceStepTrigger::ARaceStepTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	UBoxComponent * OurTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OurTriggerBox"));
	OurTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	OurTriggerBox->SetupAttachment(RootComponent);
	OurTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARaceStepTrigger::OnOverlapBegin);
	OurTriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARaceStepTrigger::OnOverlapEnd);

}

void ARaceStepTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaceStepTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaceStepTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	ARollingBall * pawn = Cast<ARollingBall>(OtherActor);
	if (pawn && (OtherActor != this) && OtherComp) {
		ARacingPlayerState * playerState = Cast<ARacingPlayerState>(pawn->GetPlayerState());
		playerState->setStep(m_step);
	}
}

void ARaceStepTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}
