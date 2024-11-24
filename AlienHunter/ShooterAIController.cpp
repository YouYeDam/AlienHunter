


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr) {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());  // AI의 시작 위치 기억
    }
}

void AShooterAIController::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
}

// AI 컨트롤러의 캐릭터 죽음을 체크하는 메소드
bool AShooterAIController::IsDead() const
{
    AMainCharacter* ControlledCharacter = Cast<AMainCharacter>(GetPawn());

    if (ControlledCharacter != nullptr) {
        return ControlledCharacter->IsDead();
    }

    return true;
}
