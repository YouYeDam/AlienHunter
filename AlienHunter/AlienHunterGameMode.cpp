// 미션 시 기본 에일리언 헌터 게임 모드


#include "AlienHunterGameMode.h"
#include "EngineUtils.h"
#include "MonsterCharacter.h"
#include "PlayerCharacter.h"
#include "GameManager.h"
#include "MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void AAlienHunterGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AAlienHunterGameMode::UpdateHUDReference()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
    {
        HUDWidget = MainPlayerController->GetHUDWidget();
    }

    // 초기 HUD 상태 업데이트
    UpdateHUDMissionProgress();
}

// 폰 처치 시 기본 처리 메소드
void AAlienHunterGameMode::PawnKilled(APawn* PawnKilled)
{
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

    if (PlayerController != nullptr)
    {
        return; // 플레이어가 죽은 경우 아래의 처리가 필요하지 않음.
    }

    AMonsterCharacter* KilledCharacter = Cast<AMonsterCharacter>(PawnKilled);
    if (KilledCharacter)
    {
        int32 Energy = KilledCharacter->GetEnergy();
        int32 EXP = KilledCharacter->GetEXP();
        
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
        
        // 플레이어가 경험치와 에너지 획득
        if (PlayerCharacter)
        {
            int32 PlayerEnergy = PlayerCharacter->GetGainedEnergy();
            int32 PlayerEXP = PlayerCharacter->GetGainedEXP();

            PlayerCharacter->SetGainedEnergy(PlayerEnergy + Energy);
            PlayerCharacter->SetGainedEXP(PlayerEXP + EXP);

            GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
            
            // 플레이어의 적 처치 수 증가
            if (GameManager) 
            {
                int32 CurrentKillEnemyCount = GameManager->GetKillEnemyCount();
                GameManager->SetKillEnemyCount(CurrentKillEnemyCount + 1);
            }
        }
    }
}

// 게임 종료 시 모든 컨트롤러에 게임 결과를 전달하는 메소드
void AAlienHunterGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}

// HUD 미션 진행 상태 갱신 메소드
void AAlienHunterGameMode::UpdateHUDMissionProgress()
{
    if (HUDWidget)
    {
        HUDWidget->UpdateMissionProgressText();
    }
}