


#include "EnemyKillGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "MeleeAIController.h"

// 게임 시작 시 모든 적의 개수 카운팅 (태그를 이용한 방법으로 전환하는 것 고려)
void AEnemyKillGameMode::BeginPlay()
{
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) 
    {
        AllEnemyCount++;
    }

    for (AMeleeAIController* Controller : TActorRange<AMeleeAIController>(GetWorld())) 
    {
        AllEnemyCount++;
    }
}

// 적 처치 시 처리 메소드
void AEnemyKillGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr) 
    {
        PlayerController->GameHasEnded(nullptr, false); // 플레이어 사망 시 게임 패배 전달
    }

    AShooterAIController* ShooterController = Cast<AShooterAIController>(PawnKilled->GetController());
    AMeleeAIController* MeleeController = Cast<AMeleeAIController>(PawnKilled->GetController());

    if (ShooterController != nullptr || MeleeController != nullptr) 
    {
        DeadEnemyCount++; // 적 사망 시 죽은 적의 수 카운팅
    }

    if (DeadEnemyCount >= AllEnemyCount) 
    {
        EndGame(true); // 모든 적 처치 시 게임 승리
    }
}

// 게임 종료 시 모든 컨트롤러에 게임 결과를 전달하는 메소드
void AEnemyKillGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld())) {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}


