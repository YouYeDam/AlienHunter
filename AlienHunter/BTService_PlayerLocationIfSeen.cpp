


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = "Update Player Location If Seen";
}

// 플레이어가 시야에 들어왔을 때 블랙보드에 위치 정보를 업데이트
void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn == nullptr) {
        return;
    }

    if (OwnerComp.GetAIOwner() == nullptr) {
        return;
    }

    // AI와 플레이어 사이의 거리 계산
    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());

    // 플레이어가 지정된 거리 내에 있고, 시야에 들어오면 위치 업데이트
    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && Distance <= DetectionRange) 
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
    }
    else // 블랙보드에서 플레이어 위치 정보를 제거
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
