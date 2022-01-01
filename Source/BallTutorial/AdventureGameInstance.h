// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureSaveGame.h"
#include "AdvancedFriendsGameInstance.h"
#include "AdventureGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BALLTUTORIAL_API UAdventureGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FString AdventureSlot = FString(TEXT("Adventure"));
	UPROPERTY(BlueprintReadWrite)
	UAdventureSaveGame * AdventureSave;

};
