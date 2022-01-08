
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActableInterface.h"
#include "CanActInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCanActInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALLTUTORIAL_API ICanActInterface
{
	GENERATED_BODY()

public:
	virtual void act() = 0;
};
