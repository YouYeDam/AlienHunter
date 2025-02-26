


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BaseAIController.h" 

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = "Update Player Location If Seen";
}

// 플레이어가 시야에 들어왔을 때 블랙보드에 위치 정보를 업데이트
void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr) 
    {
        return;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) 
    {
        return;
    }

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), AIController->GetPawn()->GetActorLocation());

    ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);

    if (AIController->LineOfSightTo(PlayerPawn) && Distance <= DetectionRange) 
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
        
        // 전투 상태 활성화
        if (BaseAIController)
        {
            BaseAIController->SetInCombat(true);
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); // 블랙보드에서 플레이어 위치 정보를 제거
    }
}
