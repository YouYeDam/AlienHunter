


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

// 슬롯을 설정하는 메소드
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

// 슬롯 클릭 시 인벤토리 위젯의 아이템 세부사항을 업데이트하는 메소드
void UInventorySlotWidget::OnSlotClicked()
{
    if (InventoryMenuWidgetRef)
    {
        InventoryMenuWidgetRef->UpdateItemDetails(ItemData);
    }
}


