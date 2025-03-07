


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BaseAIController.h" 
#include "MonsterCharacter.h"

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

    ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);
    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr)
    {
        return;
    }

    // AI와 플레이어 사이 거리 계산
    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), AIPawn->GetActorLocation());

    // 240도 시야각 제한 적용
    FVector ForwardVector = AIPawn->GetActorForwardVector(); // AI의 전방 벡터
    FVector ToPlayerVector = (PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal(); // AI -> 플레이어 방향 벡터
    float DotProduct = FVector::DotProduct(ForwardVector, ToPlayerVector); // 내적 계산

    float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(240.0f / 2.0f)); // Cos(120°) = -0.5

    // LineOfSightTo() & 거리 체크 & 시야각 체크
    if (AIController->LineOfSightTo(PlayerPawn) && Distance <= DetectionRange && DotProduct >= CosHalfFOV) 
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
        
        // 전투 상태 활성화
        if (BaseAIController)
        {
            BaseAIController->SetInCombat(true);
        }

        // 주변 몬스터들에게 전투 상태 전파
        AMonsterCharacter* Monster = Cast<AMonsterCharacter>(AIPawn);
        if (Monster)
        {
            Monster->LinkNearbyMonsters();
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); // 블랙보드에서 플레이어 위치 정보를 제거
    }
}
