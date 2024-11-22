


#include "CharacterMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMenuGameMode.h"

void UCharacterMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ShowPlayerStat();

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UCharacterMenuWidget::OnMoveToGameMenuClicked);
    }
}

// 게임 메뉴로 이동하는 메소드
void UCharacterMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

// 플레이어의 스탯을 보여주는 메소드
void UCharacterMenuWidget::ShowPlayerStat()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameManager)
    {
        if (LevelText)
        {
            FText FormattedText = FText::Format(
                NSLOCTEXT("CharacterWidget", "LevelText", "레벨: {0}"),
                FText::AsNumber(GameManager->GetPlayerLevel())
            );
            LevelText->SetText(FormattedText);
        }

        if (HealthText)
        {
            FText FormattedText = FText::Format(
                NSLOCTEXT("CharacterWidget", "HealthText", "체력: {0}"),
                FText::AsNumber(GameManager->GetInitialHealth())
            );
            HealthText->SetText(FormattedText);
        }

        if (EnergyText)
        {
            FText FormattedText = FText::Format(
                NSLOCTEXT("CharacterWidget", "EnergyText", "보유 에너지: {0}"),
                FText::AsNumber(GameManager->GetEnergy())
            );
            EnergyText->SetText(FormattedText);
        }

        if (EXPText)
        {
            FText FormattedText = FText::Format(
                NSLOCTEXT("CharacterWidget", "EXPText", "경험치: {0} / {1}"),
                FText::AsNumber(GameManager->GetEXP()),
                FText::AsNumber(GameManager->GetEXPRequirementForLevelup())
            );
            EXPText->SetText(FormattedText);
        }

        if (KillEnemyCountText)
        {
            FText FormattedText = FText::Format(
                NSLOCTEXT("CharacterWidget", "KillEnemyCountText", "처치한 적의 수: {0}"),
                FText::AsNumber(GameManager->GetKillEnemyCount())
            );
            KillEnemyCountText->SetText(FormattedText);
        }
    }
}