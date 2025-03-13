


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

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), AIPawn->GetActorLocation());

    // 240도 시야각 제한 적용
    FVector ForwardVector = AIPawn->GetActorForwardVector(); // AI의 전방 벡터
    FVector ToPlayerVector = (PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal(); // AI → 플레이어 방향 벡터
    float DotProduct = FVector::DotProduct(ForwardVector, ToPlayerVector); // 내적 계산

    float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(240.0f / 2.0f)); // Cos(120°) = -0.5
        
    // AI가 CombatStartLocation까지 이동한 후, 플레이어를 감지할 수 있는지 확인
    if (!AIController->LineOfSightTo(PlayerPawn) || Distance <= DetectionRange || DotProduct >= CosHalfFOV)
    {
        AIController->SetInCombat(false);
    }

    return EBTNodeResult::Succeeded;
}

