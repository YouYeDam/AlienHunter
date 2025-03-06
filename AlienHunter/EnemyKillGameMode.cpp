


#include "EnemyKillGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterAIController.h"
#include "MeleeAIController.h"

// 게임 시작 시 모든 적의 개수 카운팅
void AEnemyKillGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // "Enemy" 태그를 가진 모든 액터를 카운트
    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), EnemyActors);

    TotalEnemyCount = EnemyActors.Num();
    TargetEnemyCount = FMath::CeilToInt(TotalEnemyCount * 0.6f);
}

// 적 처치 시 처리 메소드
void AEnemyKillGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

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

int32 AEnemyKillGameMode::GetTargetEnemyCount() const
{
    return TargetEnemyCount;
}
