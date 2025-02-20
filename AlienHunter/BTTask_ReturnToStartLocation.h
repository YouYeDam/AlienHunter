

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReturnToStartLocation.generated.h"


UCLASS()
class ALIENHUNTER_API UBTTask_ReturnToStartLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_ReturnToStartLocation();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
