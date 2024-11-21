// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClicked);
    }
}

void UInventorySlotWidget::InitializeSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FItemData& InItemData)
{
    InventoryMenuWidgetRef = InInventoryMenuWidget;
    ItemData = InItemData;

    UTextBlock* ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemData.ItemName));
    }
}

void UInventorySlotWidget::OnSlotClicked()
{
    if (InventoryMenuWidgetRef)
    {
        InventoryMenuWidgetRef->UpdateItemDetails(ItemData);
    }
}


