


#include "ShopSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

void UShopSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UShopSlotWidget::OnSlotClicked);
    }
}

// 총기류 슬롯을 설정하는 메소드
void UShopSlotWidget::InitializeGunSlot(UShopMenuWidget* InShopMenuWidget, const FGunItemData& InGunItemData)
{
    ShopMenuWidgetRef = InShopMenuWidget;
    ItemData = InGunItemData;
    GunItemData = InGunItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemData.ItemName));
    }

    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 도검류 슬롯을 설정하는 메소드
void UShopSlotWidget::InitializeSwordSlot(UShopMenuWidget* InShopMenuWidget, const FSwordItemData& InSwordItemData)
{
    ShopMenuWidgetRef = InShopMenuWidget;
    ItemData = InSwordItemData;
    SwordItemData = InSwordItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemData.ItemName));
    }

    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 슬롯 클릭 시 상점 위젯의 아이템 세부사항을 업데이트하는 메소드
void UShopSlotWidget::OnSlotClicked()
{
    if (ShopMenuWidgetRef)
    {
        ShopMenuWidgetRef->ResetSelectedItems();

        if (ItemData.ItemType == TEXT("총기류"))
        {
            ShopMenuWidgetRef->SetSelectedGunItem(GunItemData);
        }
        else if (ItemData.ItemType == TEXT("도검류"))
        {
            ShopMenuWidgetRef->SetSelectedSwordItem(SwordItemData);
        }

        ShopMenuWidgetRef->UpdateItemDetails(ItemData);
    }
}