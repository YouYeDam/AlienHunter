


#include "BTTask_Swing.h"
#include "AIController.h"
#include "MainCharacter.h"

UBTTask_Swing::UBTTask_Swing()
{
    NodeName = TEXT("Swing");
}

// 행동 트리에서 AI가 휘두르기를 수행하는 태스크 메서드
EBTNodeResult::Type UBTTask_Swing::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) 
    {
        return EBTNodeResult::Failed;
    }
    AMainCharacter* Character = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character == nullptr) 
    {
        return EBTNodeResult::Failed;
    }
    
    Character->Swing();
    
    return EBTNodeResult::Succeeded;
}
