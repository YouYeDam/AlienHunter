


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "AlienHunterGameMode.h"
#include "PauseMenuManager.h"

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

    // 일시정지 메뉴 매니저 생성하기
    if (PauseMenuManagerClass)
    {
        PauseMenuManager = GetWorld()->SpawnActor<APauseMenuManager>(PauseMenuManagerClass);
    }
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Pause", IE_Pressed, this, &AMainPlayerController::TogglePauseMenu);
}


// 미션 결과창 레벨을 로드하는 메소드
void AMainPlayerController::LoadLevelAfterDelay()
{
    UGameplayStatics::OpenLevel(this, FName("MissionSummary"));
}

// 게임 종료 시 실행되는 메소드
void AMainPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    if (HUDWidget != nullptr) 
    {
        HUDWidget->RemoveFromParent();
    }
    
    // 일시 중지 중이라면 일시정지 상태 해제
    if (bIsPaused)
    {
        TogglePauseMenu();
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

            // 현재 미션에서 적을 처치하고 얻은 에너지와 경험치를 저장
            GameManager->SetPrevCombatEnergy(Energy);
            GameManager->SetPrevCombatEXP(EXP);

            // 처치한 적의 수 반영
            AAlienHunterGameMode* GameMode = Cast<AAlienHunterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
            if (GameMode)
            {
                int32 CurrentKillEnemyCount = GameManager->GetKillEnemyCount();
                int32 KillEnemyCount = GameMode->GetKillEnemyCount();
                GameManager->SetKillEnemyCount(CurrentKillEnemyCount + KillEnemyCount);
                GameManager->SetPrevEnemyKillCount(KillEnemyCount);
            }

            // 미션 성공한 경우, 미션 보상 지급 처리
            if (bIsWinner) 
            {
                GameManager->SetPrevMissionSuccess(true);
                GameManager->GainMissionReward();

                int32 PrevMissionEnergy = GameManager->GetCurrentMissionData().MissionEnergyReward * BonusLootMultiplier;
                int32 PrevMissionEXP = GameManager->GetCurrentMissionData().MissionEXPReward * BonusLootMultiplier;

                GameManager->SetPrevMissionEnergy(PrevMissionEnergy);
                GameManager->SetPrevMissionEXP(PrevMissionEXP);

                // 완수한 임무의 수 반영
                GameManager->SetCompleteMissionCount(GameManager->GetCompleteMissionCount() + 1);
            }
            else
            {
                GameManager->SetPrevMissionSuccess(false);

                GameManager->SetPrevMissionEnergy(0);
                GameManager->SetPrevMissionEXP(0);
            }

            // 미션 종료 시 자동 저장되도록
            GameManager->SaveGame();
        }
    }

    // 게임 종료 후 특정 시간 후에 레벨 로드
    GetWorldTimerManager().SetTimer(GameEndTimer, this, &AMainPlayerController::LoadLevelAfterDelay, GameEndDelay, false);
}

// 일시정지 메뉴를 껐다 켰다하는 메소드
void AMainPlayerController::TogglePauseMenu()
{
    if (!PauseMenuManager)
    {
        return;
    }

    if (bIsPaused)
    {
        PauseMenuManager->CloseAllMenu(); // 모든 메뉴 닫기
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter)
        {
            PlayerCharacter->EnableInput(this);
        }
    }
    else
    {
        PauseMenuManager->ShowPauseMenu(); // 일시정지 메뉴 열기
        bShowMouseCursor = true;
        SetInputMode(FInputModeGameAndUI());
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter)
        {
            PlayerCharacter->DisableInput(this);
        }
    }

    bIsPaused = !bIsPaused;
}

float AMainPlayerController::GetBonusLootMultiplier() const
{
    return BonusLootMultiplier;
}

void AMainPlayerController::SetBonusLootMultiplier(float Multiplier)
{
    BonusLootMultiplier = Multiplier;
}

UHUDWidget* AMainPlayerController::GetHUDWidget() const
{
    return HUDWidget;
}

APauseMenuManager* AMainPlayerController::GetPauseMenuManager() const
{
    return PauseMenuManager;
}

