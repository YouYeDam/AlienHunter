

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "MonsterCharacter.h"
#include "BTTask_CombatLocationWait.generated.h"

UCLASS()
class ALIENHUNTER_API UBTTask_CombatLocationWait : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:
	UBTTask_CombatLocationWait();

	void StopWait(class ABaseAIController* AIController);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
