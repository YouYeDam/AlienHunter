


#include "EnemyKillGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterAIController.h"
#include "MeleeAIController.h"

// 게임 시작 시 모든 적의 개수 카운팅 (태그를 이용한 방법으로 전환하는 것 고려)
void AEnemyKillGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // "Enemy" 태그를 가진 모든 액터를 카운트
    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), EnemyActors);

    TotalEnemyCount = EnemyActors.Num();
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
        UpdateHUDMissionProgress();
    }

    if (DeadEnemyCount >= TotalEnemyCount) 
    {
        EndGame(true); // 모든 적 처치 시 게임 승리
    }
}

int32 AEnemyKillGameMode::GetDeadEnemyCount() const
{
    return DeadEnemyCount;
}

int32 AEnemyKillGameMode::GetTotalEnemyCount() const
{
    return TotalEnemyCount;
}


