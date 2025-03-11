


#include "BTTask_Roam.h"
#include "BaseAIController.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Roam::UBTTask_Roam()
{
    NodeName = "Roam";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Roam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr) 
    {
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr) 
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (BlackboardComp == nullptr) 
    {
        return EBTNodeResult::Failed;
    }

    FVector StartLocation = BlackboardComp->GetValueAsVector(TEXT("StartLocation"));

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
    if (NavSys == nullptr) 
    {
        return EBTNodeResult::Failed;
    }

    FVector RoamLocation;
    AMonsterCharacter* MonsterCharacter = Cast<AMonsterCharacter>(AIPawn);
    if (MonsterCharacter)
    {
        RoamingRange = MonsterCharacter->GetRoamingRange();
    }

    bool bFoundLocation = NavSys->K2_GetRandomReachablePointInRadius(AIPawn->GetWorld(), StartLocation, RoamLocation, RoamingRange);
    if (!bFoundLocation)
    {
        return EBTNodeResult::Failed;
    }

    BlackboardComp->SetValueAsVector(TEXT("RoamLocation"), RoamLocation);

    AIController->StartRoaming(this);

    FAIRequestID RequestID = AIController->MoveToLocation(RoamLocation);
    if (!RequestID.IsValid())
    {
        return EBTNodeResult::Failed;
    }

    UCharacterMovementComponent* MovementComp = AIPawn->FindComponentByClass<UCharacterMovementComponent>();
    if (MovementComp)
    {
        MovementComp->MaxWalkSpeed *= 0.35f; // 이동속도 감소(걷기 모션으로 로밍)
    }

    return EBTNodeResult::InProgress;
}

// 로밍 완료를 처리하는 메소드
void UBTTask_Roam::OnTaskCompleted(ABaseAIController* AIController, EPathFollowingResult::Type Result)
{
    if (AIController == nullptr)
    {
        return;
    }

    UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
    if (BTComp == nullptr)
    {
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn)
    {
        UCharacterMovementComponent* MovementComp = AIPawn->FindComponentByClass<UCharacterMovementComponent>();
        if (MovementComp)
        {
            // 이동 속도 원래대로 복구
            MovementComp->MaxWalkSpeed *= (1 / 0.35f);
        }
    }

    if (Result == EPathFollowingResult::Success)
    {
        FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
    }
    else
    {
        FinishLatentTask(*BTComp, EBTNodeResult::Failed);
    }
}