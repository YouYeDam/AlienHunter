


#include "MissionSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UMissionSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UMissionSlotWidget::OnSlotClicked);
    }
}

// 슬롯을 설정하는 메소드
void UMissionSlotWidget::InitializeSlot(UMissionMenuWidget* InMissionMenuWidget, const FMissionData& InMissionData)
{
    MissionMenuWidgetRef = InMissionMenuWidget;
    MissionData = InMissionData;

    if (MissionNameText)
    {
        MissionNameText->SetText(FText::FromString(MissionData.MissionName));
    }
}

// 슬롯 클릭 시 미션 위젯의 미션 세부사항을 업데이트하는 메소드
void UMissionSlotWidget::OnSlotClicked()
{
    if (MissionMenuWidgetRef)
    {
        MissionMenuWidgetRef->UpdateMissionDetails(MissionData);
    }
}
