


#include "MissionSummaryWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MissionData.h"
#include "Kismet/GameplayStatics.h"

void UMissionSummaryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UMissionSummaryWidget::OnConfirmClicked);
    }
}

void UMissionSummaryWidget::ShowResult()
{
    if (!GameManager)
    {
        return;
    }

    if (MissionImage)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        if (PrevMissionData.MissionImage)
        MissionImage->SetBrushFromTexture(PrevMissionData.MissionImage);
    }

    if (MissionResultText)
    {
        if (GameManager->GetPrevMissionSuccess())
        {
            MissionResultText->SetText(FText::FromString("Mission Complete!"));
            MissionResultText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
        }
        else
        {
            MissionResultText->SetText(FText::FromString("Mission Failed!"));
            MissionResultText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
        }
    }

    if (MissionNameText)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        MissionNameText->SetText(FText::FromString(PrevMissionData.MissionName));
    }

    if (MissionRewardText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "MissionRewardText", "임무 보상 : {0} 에너지 & {1} 경험치"),
            FText::AsNumber(GameManager->GetPrevMissionEnergy()),
            FText::AsNumber(GameManager->GetPrevMissionEXP())
        );
        MissionRewardText->SetText(FormattedText);
    }

    if (GainedEnergyText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "GainedEnergyText", "획득한 총 에너지: {0} + {1}"),
            FText::AsNumber(GameManager->GetPrevMissionEnergy()),
            FText::AsNumber(GameManager->GetPrevCombatEnergy())
        );
        GainedEnergyText->SetText(FormattedText);
    }

    if (GainedEXPText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "GainedEXPText", "획득한 총 경험치: {0} + {1}"),
            FText::AsNumber(GameManager->GetPrevMissionEXP()),
            FText::AsNumber(GameManager->GetPrevCombatEXP())
        );
        GainedEXPText->SetText(FormattedText);
    }

    if (KillEnemyCountText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "KillEnemyCountText", "처치한 적의 수: {0}"),
            FText::AsNumber(GameManager->GetPrevEnemyKillCount())
        );
        KillEnemyCountText->SetText(FormattedText);
    }
}

// 확인 버튼 클릭 후 게임 메뉴 레벨을 로드하는 메소드
void UMissionSummaryWidget::OnConfirmClicked()
{
    UGameplayStatics::OpenLevel(this, FName("GameMenu"));
}
