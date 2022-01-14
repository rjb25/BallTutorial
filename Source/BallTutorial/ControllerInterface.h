#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALLTUTORIAL_API IControllerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Unreliable, WithValidation)
	virtual void ServerHurt(AActor* toHurt, float pain) = 0;
	virtual bool ServerHurt_Validate(AActor* toHurt, float pain) = 0;
	virtual void ServerHurt_Implementation(AActor* toHurt, float pain) = 0;
};
