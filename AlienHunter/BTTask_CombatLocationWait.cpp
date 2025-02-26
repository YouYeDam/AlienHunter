


#include "BTTask_CombatLocationWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_CombatLocationWait::UBTTask_CombatLocationWait()
{
	NodeName = "Combat Location Wait";
}

EBTNodeResult::Type UBTTask_CombatLocationWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); // 기본 Wait 동작 실행

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	MonsterCharacter = Cast<AMonsterCharacter>(AIController->GetPawn()); // AI가 조종하는 몬스터 가져오기
	if (MonsterCharacter)
	{
		// 몬스터 피격 이벤트 구독
		MonsterCharacter->OnMonsterDamaged.AddDynamic(this, &UBTTask_CombatLocationWait::OnMonsterDamaged);
	}

	return Result;
}

// 몬스터가 피격되었을 때 즉시 Wait을 종료하는 메소드
void UBTTask_CombatLocationWait::OnMonsterDamaged()
{
    if (MonsterCharacter)
    {
        MonsterCharacter->OnMonsterDamaged.RemoveDynamic(this, &UBTTask_CombatLocationWait::OnMonsterDamaged);

        AAIController* AIController = Cast<AAIController>(MonsterCharacter->GetController());
        if (AIController)
        {
            UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
            if (BTComp)
            {
                FinishLatentTask(*BTComp, EBTNodeResult::Succeeded); // 현재 Task 강제 종료
                BTComp->RestartLogic(); // Behavior Tree 즉시 업데이트 요청
            }
        }
    }
}