


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
        float Angle = FMath::RandRange(-35.0f, 35.0f); // 위쪽(상반원) 범위 내 랜덤 각도
        float Distance = 350.0f;
    
        FVector Offset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), 
                                 FMath::Sin(FMath::DegreesToRadians(Angle)), 
                                 0.0f) * Distance; // X-Y 평면에서 방향 조정
        FVector TargetLocation = CombatStartLocation + Offset;
    
        FNavLocation NavLocation;
        if (NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
        {
            AIController->MoveToLocation(NavLocation.Location);
            return EBTNodeResult::Succeeded;
        }
    }
    
    return EBTNodeResult::Failed;
}
