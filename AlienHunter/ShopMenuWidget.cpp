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

    CreateGunInventorySlots();  // 아이템 슬롯 초기화
 
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

    if (GunTabButton)
    {
        GunTabButton->OnClicked.AddDynamic(this, &UShopMenuWidget::OnGunTabClicked);
    }

    if (SwordTabButton)
    {
        SwordTabButton->OnClicked.AddDynamic(this, &UShopMenuWidget::OnSwordTabClicked);
    }
}

// 아이템 데이터를 초기화하는 함수
void UShopMenuWidget::InitializeItemData()
{
    if (!GunItemDataTable || !SwordItemDataTable)
    {
        return;
    }

    static const FString GunContextString(TEXT("Shop Menu Gun Item Initialization"));
    static const FString SwordContextString(TEXT("Shop Menu Sword Item Initialization"));

    TArray<FGunItemData*> GunItems;
    TArray<FSwordItemData*> SwordItems;

    // 총기류 데이터 테이블에서 모든 행 가져오기
    GunItemDataTable->GetAllRows(GunContextString, GunItems);
    for (FGunItemData* GunItem : GunItems)
    {
        if (GunItem)
        {
            GunItemDataArray.Add(*GunItem); // 총기류 배열에 추가
        }
    }

    // 도검류 데이터 테이블에서 모든 행 가져오기
    SwordItemDataTable->GetAllRows(SwordContextString, SwordItems);
    for (FSwordItemData* SwordItem : SwordItems)
    {
        if (SwordItem)
        {
            SwordItemDataArray.Add(*SwordItem); // 도검류 배열에 추가
        }
    }
}

// 총기류 상점 슬롯을 생성하고 초기화하는 메소드
void UShopMenuWidget::CreateGunInventorySlots()
{
    if (!ShopSlotClass || !ShopScrollBox)
    {
        return;
    }

    ShopScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 총기류 슬롯 생성
    for (const FGunItemData& GunItem : GunItemDataArray)
    {
        UShopSlotWidget* GunSlot = CreateWidget<UShopSlotWidget>(this, ShopSlotClass);
        if (GunSlot)
        {
            GunSlot->InitializeGunSlot(this, GunItem);
            ShopScrollBox->AddChild(GunSlot);
        }
    }
}

// 도검류 상점 슬롯을 생성하고 초기화하는 메소드
void UShopMenuWidget::CreateSwordInventorySlots()
{
    if (!ShopSlotClass || !ShopScrollBox)
    {
        return;
    }

    ShopScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 도검류 슬롯 생성
    for (const FSwordItemData& SwordItem : SwordItemDataArray)
    {
        UShopSlotWidget* SwordSlot = CreateWidget<UShopSlotWidget>(this, ShopSlotClass);
        if (SwordSlot)
        {
            SwordSlot->InitializeSwordSlot(this, SwordItem);
            ShopScrollBox->AddChild(SwordSlot);
        }
    }
}

// 현재 보유 에너지를 보여주는 메소드
void UShopMenuWidget::ShowCurrentEnergy()
{
    if (EnergyAmountText && GameManager)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "EnergyText", "보유 에너지: {0}"),
            FText::AsNumber(GameManager->GetEnergy())
        );
        EnergyAmountText->SetText(FormattedText);
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

	if (GameManager->GetEnergy() < SelectedItem.ItemPrice) // 보유 에너지가 아이템 가격보다 낮으면 구매 불가
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
    else if (CheckItemAlreadyPurchased()) // 이미 아이템을 구매한 상태라면 구매 불가
    {
        if (PopupWidgetClass)
        {
            FText FormattedText = NSLOCTEXT("ShopMenu", "AlreadyPurchased", "이미 구매한 아이템입니다!");

            PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
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

void UShopMenuWidget::OnGunTabClicked()
{
    CreateGunInventorySlots();
}

void UShopMenuWidget::OnSwordTabClicked()
{
    CreateSwordInventorySlots();
}

// 구매 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UShopMenuWidget::OnConfirmPurchase()
{
	if (GameManager)
	{
		int32 CurrentEnergy = GameManager->GetEnergy();
		GameManager->SetEnergy(CurrentEnergy -SelectedItem.ItemPrice);

        if (SelectedItem.ItemType == TEXT("총기류"))
        {
            GameManager->AddPurchasedGunItem(SelectedGunItem);
        }
        else if (SelectedItem.ItemType == TEXT("도검류")) 
        {
            GameManager->AddPurchasedSwordItem(SelectedSwordItem);
        }

        ShowCurrentEnergy();
	}

    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UShopMenuWidget::OnConfirmPurchase);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UShopMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }

    // 구매한 아이템을 바로 장착할 것인지 확인
    if (PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
        if (PopupWidget)
        {
            FText EquipMessage = NSLOCTEXT("ShopMenu", "EquipConfirmation", "구매한 아이템을 장착하시겠습니까?");
            
            PopupWidget->InitializePopup(EquipMessage, true);

            PopupWidget->ConfirmClicked.AddDynamic(this, &UShopMenuWidget::OnConfirmEquip);
            PopupWidget->CancelClicked.AddDynamic(this, &UShopMenuWidget::OnPopupClose);

            PopupWidget->AddToViewport();
        }
    }
}

// 장비 장착 확인 버튼 클릭 시 실행되는 메소드
void UShopMenuWidget::OnConfirmEquip()
{
	if (!GameManager || SelectedItem.ItemName.IsEmpty())
	{
		return;
	}

    // 게임 매니저에서 장착중인 무기의 아이템 데이터와 액터 바꿔주기
    if (SelectedItem.ItemType == TEXT("총기류")) 
    {
        GameManager->SetEquippedGunItemData(SelectedGunItem);
        GameManager->SetEquippedGun(SelectedItem.ItemBlueprint);
    }
    else if (SelectedItem.ItemType == TEXT("도검류")) 
    {
        GameManager->SetEquippedSwordItemData(SelectedSwordItem);
        GameManager->SetEquippedSword(SelectedItem.ItemBlueprint);
    }
    
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UShopMenuWidget::OnConfirmEquip);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UShopMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
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
void UShopMenuWidget::UpdateItemDetails(const FBaseItemData& ItemData)
{
    SelectedItem = ItemData;

    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemData.ItemName));
    }
    if (ItemTypeText)
    {
        ItemTypeText->SetText(FText::FromString(ItemData.ItemType.ToString()));
    }
    if (ItemDescriptionText)
    {
        ItemDescriptionText->SetText(FText::FromString(ItemData.ItemDescription));
    }
    if (ItemPriceText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "ItemPriceText", "에너지: {0}"),
            FText::AsNumber(ItemData.ItemPrice)
        );
        ItemPriceText->SetText(FormattedText);
    }
    if (ItemDamageText)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("ShopMenu", "ItemDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        ItemDamageText->SetText(FormattedText);
    }
    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 아이템을 이미 구매했는지 확인하는 메소드
bool UShopMenuWidget::CheckItemAlreadyPurchased() const
{
    // 총기류 아이템에서 중복 확인
    for (const FGunItemData& GunItem : GameManager->GetPurchasedGunItems())
    {
        if (GunItem.ItemName == SelectedItem.ItemName)
        {
            return true;
        }
    }

    // 도검류 아이템에서 중복 확인
    for (const FSwordItemData& SwordItem : GameManager->GetPurchasedSwordItems())
    {
        if (SwordItem.ItemName == SelectedItem.ItemName)
        {
            return true;
        }
    }

    return false;
}

// 빈 데이터로 초기화하는 메소드
void UShopMenuWidget::ResetSelectedItems()
{
    SelectedGunItem = FGunItemData();
    SelectedSwordItem = FSwordItemData();
}

void UShopMenuWidget::SetSelectedGunItem(const FGunItemData& GunItem)
{
    SelectedGunItem = GunItem;
}

void UShopMenuWidget::SetSelectedSwordItem(const FSwordItemData& SwordItem)
{
    SelectedSwordItem = SwordItem;
}