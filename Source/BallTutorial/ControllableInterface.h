#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UControllableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALLTUTORIAL_API IControllableInterface
{
	GENERATED_BODY()

public:
	virtual void moveRight(float AxisValue) = 0;
	virtual void moveLeft(float AxisValue) = 0;
	virtual void moveForward(float AxisValue) = 0;
	virtual void moveBack(float AxisValue) = 0;
	virtual void rotateRight(float AxisValue) = 0;
	virtual void rotateLeft(float AxisValue) = 0;
	virtual void jump() = 0;
	virtual void slow(float AxisValue) = 0;
	virtual void boost(float AxisValue) = 0;
};
