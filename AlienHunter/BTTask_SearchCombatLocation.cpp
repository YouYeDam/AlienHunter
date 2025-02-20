


#include "BTTask_SearchCombatLocation.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_SearchCombatLocation::UBTTask_SearchCombatLocation()
{
    this->NodeName = "Search Around Combat Location";
}

EBTNodeResult::Type UBTTask_SearchCombatLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector CombatStartLocation = BlackboardComp->GetValueAsVector(TEXT("CombatStartLocation"));
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());

    if (NavSys)
    {
        UE_LOG(LogTemp, Warning, TEXT("찾는중"));
        FNavLocation RandomPoint;
        if (NavSys->GetRandomPointInNavigableRadius(CombatStartLocation, 500.0f, RandomPoint))
        {
            AIController->MoveToLocation(RandomPoint.Location);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Succeeded;
}
