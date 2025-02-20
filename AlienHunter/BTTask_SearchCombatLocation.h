


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SearchCombatLocation.generated.h"

UCLASS()
class ALIENHUNTER_API UBTTask_SearchCombatLocation : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SearchCombatLocation();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
