

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_Roam.generated.h"

UCLASS()
class ALIENHUNTER_API UBTTask_Roam : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Roam();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	AAIController* AIController;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
