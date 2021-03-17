// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RacingWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API URacingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock * Lap;
	UPROPERTY(meta = (BindWidget))
		UTextBlock * Time;
	UPROPERTY(meta = (BindWidget))
		UTextBlock * Place;
	void setTime(float time);
	void setLap(int lap);
	void setPlace(int place);
};
