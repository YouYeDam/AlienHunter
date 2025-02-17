


#include "MeleeAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SwordMonsterCharacter.h"

void AMeleeAIController::BeginPlay()
{
    Super::BeginPlay();
    
    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); // AI의 시작 위치 기억
    }
}

void AMeleeAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// AI 컨트롤러의 캐릭터 죽음을 체크하는 메소드
bool AMeleeAIController::IsDead() const
{
    ASwordMonsterCharacter* ControlledCharacter = Cast<ASwordMonsterCharacter>(GetPawn());

    if (ControlledCharacter != nullptr) {
        return ControlledCharacter->IsDead();
    }

    return true;
}
