


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

	void OnTaskCompleted(class ABaseAIController* AIController, EPathFollowingResult::Type Result);

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	float RoamingRange = 1000.0f;
};