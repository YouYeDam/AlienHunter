


#include "BTTask_Roam.h"
#include "BaseAIController.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Roam::UBTTask_Roam()
{
    NodeName = "Roam";
}

EBTNodeResult::Type UBTTask_Roam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIController = OwnerComp.GetAIOwner();
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
    bool bFoundLocation = NavSys->K2_GetRandomReachablePointInRadius(AIPawn->GetWorld(), StartLocation, RoamLocation, 1000.0f);
    
    if (bFoundLocation == false)
    {
        return EBTNodeResult::Failed;
    }

    BlackboardComp->SetValueAsVector(TEXT("RoamLocation"), RoamLocation);

    FAIRequestID RequestID = AIController->MoveToLocation(RoamLocation);

    if (RequestID.IsValid() == false)
    {
        return EBTNodeResult::Failed;
    }

    UCharacterMovementComponent* MovementComp = AIPawn->FindComponentByClass<UCharacterMovementComponent>();
    if (MovementComp)
    {
        MovementComp->MaxWalkSpeed *= 0.35f; // 이동속도 감소(걷기 모션으로 로밍)
    }

    AIController->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_Roam::OnMoveCompleted);

    return EBTNodeResult::InProgress;
}

void UBTTask_Roam::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (AIController == nullptr)
    {
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn)
    {
        UCharacterMovementComponent* MovementComp = AIPawn->FindComponentByClass<UCharacterMovementComponent>();
        if (MovementComp)
        {
            MovementComp->MaxWalkSpeed *= (1 / 0.35f); // 원래 속도로 복구
        }
    }

    UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
    if (BTComp == nullptr) 
    {
        return;
    }
    if (Result == EPathFollowingResult::Success) 
    {
        FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
    }
    else 
    {
        FinishLatentTask(*BTComp, EBTNodeResult::Failed);
    }

    AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_Roam::OnMoveCompleted);
}

