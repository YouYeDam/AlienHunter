

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckCombatState.generated.h"


UCLASS()
class ALIENHUNTER_API UBTTask_CheckCombatState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_CheckCombatState();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 DetectionRange = 3000.0f; // AI의 플레이어 탐지 가능 거리
};
