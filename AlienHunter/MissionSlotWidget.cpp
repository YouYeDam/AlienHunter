// Fill out your copyright notice in the Description page of Project Settings.


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

void UMissionSlotWidget::InitializeSlot(UMissionMenuWidget* InMissionMenuWidget, const FMissionData& InMissionData)
{
    MissionMenuWidgetRef = InMissionMenuWidget;
    MissionData = InMissionData;

    UTextBlock* MissionNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionName")));
    if (MissionNameText)
    {
        MissionNameText->SetText(FText::FromString(MissionData.MissionName));
    }
}

void UMissionSlotWidget::OnSlotClicked()
{
    if (MissionMenuWidgetRef)
    {
        MissionMenuWidgetRef->UpdateMissionDetails(MissionData);
    }
}
