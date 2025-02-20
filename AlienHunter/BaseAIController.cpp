


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
