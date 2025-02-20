


#include "BTTask_ReturnToStartLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAIController.h"

UBTTask_ReturnToStartLocation::UBTTask_ReturnToStartLocation()
{
    NodeName = "Return To Start Location";
}

EBTNodeResult::Type UBTTask_ReturnToStartLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    
    if (!AIController || !BlackboardComp) 
    {
        return EBTNodeResult::Failed;
    }

    FVector StartLocation = BlackboardComp->GetValueAsVector(TEXT("StartLocation"));
    AIController->MoveToLocation(StartLocation);

    // 전투 상태 비활성화
    ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
    if (BaseAIController)
    {
        //BaseAIController->SetInCombat(false);
    }

    return EBTNodeResult::Succeeded;
}
