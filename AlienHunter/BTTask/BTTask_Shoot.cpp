


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "GunMonsterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

// 행동 트리에서 AI가 사격을 수행하는 태스크 메서드
EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) 
    {
        return EBTNodeResult::Failed;
    }
    
    AGunMonsterCharacter* Character = Cast<AGunMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character == nullptr) 
    {
        return EBTNodeResult::Failed;
    }
    
    Character->Shoot();
    
    return EBTNodeResult::Succeeded;
}