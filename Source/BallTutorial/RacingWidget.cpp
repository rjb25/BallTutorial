// Fill out your copyright notice in the Description page of Project Settings.

#include "RacingWidget.h"
#include "Components/TextBlock.h"
	void URacingWidget::setTime(float time) {
		if (Time) {
			Time->SetText(FText::AsNumber(time));
		}
	}
	void URacingWidget::setLap(int lap) {
		if (Lap) {
			Lap->SetText(FText::AsNumber(lap));
		}

	}
	void URacingWidget::setPlace(int place) {
		if (Place) {
			Place->SetText(FText::AsNumber(place));
		}
	}
