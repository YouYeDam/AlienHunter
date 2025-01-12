


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "EnemyKillGameMode.h"

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUDWidget = CreateWidget<UHUDWidget>(this, HUDClass);
    if (HUDWidget != nullptr) 
    {
        HUDWidget->AddToViewport(); // HUD 띄우기
    }

    // GameMode에 HUD 참조 업데이트
    if (AAlienHunterGameMode* GameMode = Cast<AAlienHunterGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->UpdateHUDReference();
    }
}

// 게임 메뉴 레벨을 로드하는 메소드
void AMainPlayerController::LoadLevelAfterDelay()
{
    UGameplayStatics::OpenLevel(this, FName("GameMenu"));
}

// 게임 종료 시 실행되는 메소드
void AMainPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    if (HUDWidget != nullptr) 
    {
        HUDWidget->RemoveFromParent();
    }
    
    // 승리 또는 패배 화면 표시
    if (bIsWinner) {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr) {
            WinScreen->AddToViewport();
        }
    }
    else {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr) {
            LoseScreen->AddToViewport();
        }
    }

    // 플레이어 캐릭터의 에너지 및 경험치 가져오기
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (PlayerCharacter)
    {
        int32 Energy = PlayerCharacter->GetGainedEnergy(); // 획득한 에너지
        int32 EXP = PlayerCharacter->GetGainedEXP(); // 획득한 경험치

        GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

        if (GameManager)
        {
            int32 CurrentEnergy = GameManager->GetEnergy(); // 현재 에너지
            int32 CurrentEXP = GameManager->GetEXP(); // 현재 경험치

            // 플레이어의 에너지와 경험치를 게임 매니저에 추가
            GameManager->SetEnergy(CurrentEnergy + Energy);
            GameManager->SetEXP(CurrentEXP + EXP);

            // 미션 성공한 경우, 미션 보상 지급 처리
            if (bIsWinner) 
            {
                GameManager->GainMissionReward();
            }
        }
    }

    // 게임 종료 후 특정 시간 후에 레벨 로드
    GetWorldTimerManager().SetTimer(GameEndTimer, this, &AMainPlayerController::LoadLevelAfterDelay, GameEndDelay, false);
}

UHUDWidget* AMainPlayerController::GetHUDWidget() const
{
    return HUDWidget;
}
