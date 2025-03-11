


#include "BTTask_CombatLocationWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAIController.h"

UBTTask_CombatLocationWait::UBTTask_CombatLocationWait()
{
	NodeName = "Combat Location Wait";
}

EBTNodeResult::Type UBTTask_CombatLocationWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); // 기본 Wait 동작 실행

	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

    AIController->StartCheckingCombatLocationWait(this);

	return Result;
}

// 몬스터가 피격되었을 때 즉시 Wait을 종료하는 메소드
void UBTTask_CombatLocationWait::StopWait(class ABaseAIController* AIController)
{
    if (AIController)
    {
        UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
        if (BTComp)
        {
            FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
            BTComp->RestartLogic(); // 행동 트리 즉시 업데이트 요청
        }
    }
}