// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UShopSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UShopSlotWidget::OnSlotClicked);
    }
}

void UShopSlotWidget::InitializeSlot(UShopMenuWidget* InShopMenuWidget, const FItemData& InItemData)
{
    ShopMenuWidgetRef = InShopMenuWidget;
    ItemData = InItemData;

    UTextBlock* ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemData.ItemName));
    }
}

void UShopSlotWidget::OnSlotClicked()
{
    if (ShopMenuWidgetRef)
    {
        ShopMenuWidgetRef->UpdateItemDetails(ItemData);
    }
}


