


#include "InventorySlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClicked);
    }
}

// 총기류 슬롯을 설정하는 메소드
void UInventorySlotWidget::InitializeGunSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FGunItemData& InGunItemData)
{
    InventoryMenuWidgetRef = InInventoryMenuWidget;
    ItemData = InGunItemData;
    GunItemData = InGunItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(GunItemData.ItemName));
    }

    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 도검류 슬롯을 설정하는 메소드
void UInventorySlotWidget::InitializeSwordSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FSwordItemData& InSwordItemData)
{
    InventoryMenuWidgetRef = InInventoryMenuWidget;
    ItemData = InSwordItemData;
    SwordItemData = InSwordItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(SwordItemData.ItemName));
    }

    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 수류탄류 슬롯을 설정하는 메소드
void UInventorySlotWidget::InitializeGrenadeSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FGrenadeItemData& InGrenadeItemData)
{
    InventoryMenuWidgetRef = InInventoryMenuWidget;
    ItemData = InGrenadeItemData;
    GrenadeItemData = InGrenadeItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(GrenadeItemData.ItemName));
    }

    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 슬롯 클릭 시 인벤토리 위젯의 아이템 세부사항을 업데이트하는 메소드
void UInventorySlotWidget::OnSlotClicked()
{
    if (InventoryMenuWidgetRef)
    {
        InventoryMenuWidgetRef->ResetSelectedItems();

        if (ItemData.ItemType == TEXT("총기류"))
        {
            InventoryMenuWidgetRef->SetSelectedGunItem(GunItemData);
        }
        else if (ItemData.ItemType == TEXT("도검류"))
        {
            InventoryMenuWidgetRef->SetSelectedSwordItem(SwordItemData);
        }
        else if (ItemData.ItemType == TEXT("수류탄류"))
        {
            InventoryMenuWidgetRef->SetSelectedGrenadeItem(GrenadeItemData);
        }

        InventoryMenuWidgetRef->UpdateItemDetails(ItemData);
    }
}


