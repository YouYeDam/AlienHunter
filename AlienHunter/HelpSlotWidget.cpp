


#include "HelpSlotWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UHelpSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

// 도움말 정보 슬롯을 설정하는 메소드
void UHelpSlotWidget::InitializeSlot(UHelpMenuWidget* InHelpMenuWidgetRef, const FHelpInfoData& InHelpInfoData)
{
    HelpMenuWidgetRef = InHelpMenuWidgetRef;
    HelpInfoData = InHelpInfoData;

    if (HelpInfoText)
    {
        HelpInfoText->SetText(FText::FromString(HelpInfoData.HelpInfoName + " : " + HelpInfoData.HelpInfoNameDescription));
    }
}
