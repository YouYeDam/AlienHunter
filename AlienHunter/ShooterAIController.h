

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "ShooterAIController.generated.h"


UCLASS()
class ALIENHUNTER_API AShooterAIController : public ABaseAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
