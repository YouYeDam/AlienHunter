


#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

ABaseAIController::ABaseAIController()
{

}

void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr) 
    {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); // AI의 시작 위치 기억
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCombat"), false); // 전투 상태 초기화
    }
}

void ABaseAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// AI 몬스터의 전투 중 상태를 활성화하는 메소드
void ABaseAIController::SetInCombat(bool Value)
{
    bIsInCombat = Value;

    GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCombat"), Value);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        FVector CombatStartLocation = PlayerPawn->GetActorLocation();
        GetBlackboardComponent()->SetValueAsVector(TEXT("CombatStartLocation"), CombatStartLocation);
    }
}

// AI 컨트롤러의 캐릭터 죽음을 체크하는 메소드
bool ABaseAIController::IsDead() const
{
    AMainCharacter* ControlledCharacter = Cast<AMainCharacter>(GetPawn());

    if (ControlledCharacter != nullptr) 
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}

// 로밍을 시작할 때 태스크를 저장하는 메소드
void ABaseAIController::StartRoaming(UBTTask_Roam* Task)
{
    if (!Task)
    {
        return;
    }

    CurrentTask_Roam = Task;
    ReceiveMoveCompleted.AddDynamic(this, &ABaseAIController::OnMoveCompleted);
}

// 로밍 이동이 끝나고 결과를 전달하는 메소드
void ABaseAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (CurrentTask_Roam)
    {
        CurrentTask_Roam->OnTaskCompleted(this, Result);
    }

    ReceiveMoveCompleted.RemoveDynamic(this, &ABaseAIController::OnMoveCompleted);
    CurrentTask_Roam = nullptr;
}

// 피격 장소에서 대기를 시작할 때 태스크를 저장하는 메소드
void ABaseAIController::StartCheckingCombatLocationWait(UBTTask_CombatLocationWait* Task)
{
    if (!Task)
    {
        return;
    }

    CurrentTask_CombatLocationWait = Task;
}

 // 몬스터가 피격될 때 실행되는 메소드
void ABaseAIController::OnMonsterDamaged()
{
    if (CurrentTask_CombatLocationWait)
    {
        CurrentTask_CombatLocationWait->StopWait(this);
    }

    CurrentTask_CombatLocationWait = nullptr;
}