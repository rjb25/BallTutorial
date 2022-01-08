#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALLTUTORIAL_API IActableInterface
{
	GENERATED_BODY()

public:
	virtual void act(FVector Direction) = 0;
};
