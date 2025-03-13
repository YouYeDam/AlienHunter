

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Swing.generated.h"


UCLASS()
class ALIENHUNTER_API UBTTask_Swing : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Swing();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
