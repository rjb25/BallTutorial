// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RacingHUD.generated.h"

class UUserWidget;
class ARacingGameState;
class APlayerController;
/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API ARacingHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	ARacingHUD();

	void createWidgets();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> m_raceStats;
	//Change this to a cpp blueprint widget that they inherit from
	UPROPERTY()
	UUserWidget * m_raceStatsWidget;
	
};
