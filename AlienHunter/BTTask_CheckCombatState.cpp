


#include "BTTask_CheckCombatState.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_CheckCombatState::UBTTask_CheckCombatState()
{
    NodeName = "Check Combat State";
}

EBTNodeResult::Type UBTTask_CheckCombatState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIController->GetWorld(), 0);
    if (PlayerPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), AIController->GetPawn()->GetActorLocation());

    // AI가 CombatStartLocation까지 이동한 후, 플레이어를 감지할 수 있는지 확인
    if (!AIController->LineOfSightTo(PlayerPawn) && Distance <= DetectionRange)
    {
        AIController->SetInCombat(false);
    }

    return EBTNodeResult::Succeeded;
}
