// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopMenuWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShopSlotWidget.h"
#include "PopupWidget.h"
#include "GameMenuGameMode.h"

void UShopMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeItemData();  // 아이템 데이터 초기화

    CreateItemSlots();  // 아이템 슬롯 초기화
 
    ShowCurrentEnergy();
    
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UShopMenuWidget::OnMoveToGameMenuClicked);
    }
    if (BuyButton)
    {
        BuyButton->OnClicked.AddDynamic(this, &UShopMenuWidget::OnBuyItemClicked);
    }
}

// 아이템 데이터를 초기화하는 함수
void UShopMenuWidget::InitializeItemData()
{
    if (!ShopItemDataTable)
    {
        return;
    }

    static const FString ContextString(TEXT("Shop Menu Item Initialization"));
    TArray<FItemData*> ShopItems;

    // 데이터 테이블에서 모든 행 가져오기
    ShopItemDataTable->GetAllRows(ContextString, ShopItems);

    for (FItemData* Item : ShopItems)
    {
        if (Item)
        {
            ItemDataArray.Add(*Item); // 배열에 아이템 추가
        }
    }
}

// 상점 슬롯을 생성하고 초기화하는 메소드
void UShopMenuWidget::CreateItemSlots()
{
    if (!ShopSlotClass || !ShopScrollBox)
    {
        return;
    }

    // 아이템 데이터를 순회하며 슬롯을 생성하고 초기화
    for (const FItemData& Item : ItemDataArray)
    {
        UShopSlotWidget* ShopSlot = CreateWidget<UShopSlotWidget>(this, ShopSlotClass);
        if (ShopSlot)
        {
            if (ShopSlot)
            {
                ShopSlot->InitializeSlot(this, Item);
            }

            ShopScrollBox->AddChild(ShopSlot);
        }
    }
}

// 현재 보유 에너지를 보여주는 메소드
void UShopMenuWidget::ShowCurrentEnergy()
{
    if (EnergyAmount && GameManager)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "EnergyText", "보유 에너지: {0}"),
            FText::AsNumber(GameManager->GetEnergy())
        );
        EnergyAmount->SetText(FormattedText);
    }
}

// 게임 메뉴로 이동하는 메소드
void UShopMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

// 구매 버튼 클릭 시 실행되는 메소드
void UShopMenuWidget::OnBuyItemClicked()
{
	if (SelectedItem.ItemName.IsEmpty())
	{
		return;
	}

	if (GameManager->GetEnergy() < SelectedItem.ItemPrice)
	{
		if (PopupWidgetClass)
		{
            FText FormattedText = NSLOCTEXT("ShopMenu", "InsufficientEnergy", "에너지가 부족합니다!");

			PopupWidget  = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (PopupWidget)
			{
				PopupWidget->AddToViewport();
				PopupWidget->InitializePopup(FormattedText, false);

				PopupWidget->ConfirmClicked.AddDynamic(this, &UShopMenuWidget::OnPopupClose);
			}
		}
	}
	else
	{
		if (PopupWidgetClass)
		{
			PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (PopupWidget)
			{
                FText FormattedText = NSLOCTEXT("ShopMenu", "PurchaseConfirmation", "구매하시겠습니까?");

				PopupWidget->AddToViewport();
				PopupWidget->InitializePopup(FormattedText, true);

				PopupWidget->ConfirmClicked.AddDynamic(this, &UShopMenuWidget::OnConfirmPurchase);
				PopupWidget->CancelClicked.AddDynamic(this, &UShopMenuWidget::OnPopupClose);
			}
		}
	}
}

// 구매 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UShopMenuWidget::OnConfirmPurchase()
{
	if (GameManager)
	{
		int32 CurrentEnergy = GameManager->GetEnergy();
		GameManager->SetEnergy(CurrentEnergy -SelectedItem.ItemPrice);

        GameManager->AddPurchasedItem(SelectedItem);

        ShowCurrentEnergy();
	}

    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UShopMenuWidget::OnConfirmPurchase);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UShopMenuWidget::OnPopupClose);
        PopupWidget = nullptr;
    }
}

// 구매 버튼 클릭 후 취소 버튼 클릭 시 실행되는 메소드
void UShopMenuWidget::OnPopupClose()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UShopMenuWidget::OnConfirmPurchase);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UShopMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}

// 선택된 아이템과 세부사항 UI를 업데이트하는 메소드
void UShopMenuWidget::UpdateItemDetails(const FItemData& ItemData)
{
    SelectedItem = ItemData;

    if (ItemName)
    {
        ItemName->SetText(FText::FromString(ItemData.ItemName));
    }
    if (ItemType)
    {
        ItemType->SetText(FText::FromString(ItemData.ItemType.ToString()));
    }
    if (ItemDescription)
    {
        ItemDescription->SetText(FText::FromString(ItemData.ItemDescription));
    }
    if (ItemPrice)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "ItemPriceText", "에너지: {0}"),
            FText::AsNumber(ItemData.ItemPrice)
        );
        ItemPrice->SetText(FormattedText);
    }
    if (ItemDamage)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "ItemDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        ItemDamage->SetText(FormattedText);
    }
    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}





