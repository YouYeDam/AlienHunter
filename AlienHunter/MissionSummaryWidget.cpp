


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
        }
        else
        {
            MissionResultText->SetText(FText::FromString("Mission Failed!"));
        }
    }

    if (MissionNameText)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        MissionNameText->SetText(FText::FromString(PrevMissionData.MissionName));
    }

    if (MissionRewardText)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "MissionRewardText", "에너지 : {0}   EXP : {1}"),
            FText::AsNumber(PrevMissionData.MissionEnergyReward),
            FText::AsNumber(PrevMissionData.MissionEXPReward)
        );
        MissionRewardText->SetText(FormattedText);
    }

    if (GainedEnergyText)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "GainedEnergyText", "획득한 에너지: {0} + {1}"),
            FText::AsNumber(PrevMissionData.MissionEnergyReward),
            FText::AsNumber(GameManager->GetPrevMissionEnergy())
        );
        GainedEnergyText->SetText(FormattedText);
    }

    if (GainedEXPText)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionSummary", "GainedEXPText", "획득한 경험치: {0} + {1}"),
            FText::AsNumber(PrevMissionData.MissionEXPReward),
            FText::AsNumber(GameManager->GetPrevMissionEXP())
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
