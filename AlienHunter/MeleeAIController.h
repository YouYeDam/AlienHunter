

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "MeleeAIController.generated.h"


UCLASS()
class ALIENHUNTER_API AMeleeAIController : public ABaseAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
