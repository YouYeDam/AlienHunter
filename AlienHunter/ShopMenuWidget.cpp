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

    InitializeItemData();

    // 슬롯 동적 생성
    CreateItemSlots();

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

void UShopMenuWidget::InitializeItemData()
{
    FItemData Item1;
    Item1.ItemName = TEXT("런쳐 건");
    Item1.ItemDescription = TEXT("라이플 건보다 강화된 런쳐 건입니다.");
    Item1.ItemType = TEXT("총기류");
    Item1.ItemPrice = 100;
    Item1.ItemDamage = 20;
    Item1.ItemImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Weapon/Launcher.Launcher")));
    Item1.ItemBlueprint = LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/Gun/BP_Launcher.BP_Launcher_C"));

    FItemData Item2;
    Item2.ItemName = TEXT("사이버 소드");
    Item2.ItemDescription = TEXT("밝게 빛나는 사이버 소드입니다.");
    Item2.ItemType = TEXT("도검류");
    Item2.ItemPrice = 150;
    Item2.ItemDamage = 40;
    Item2.ItemImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Weapon/CyberSword.CyberSword")));
    Item2.ItemBlueprint = LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/Sword/BP_CyberSword.BP_CyberSword_C"));

    FItemData Item3;
    Item3.ItemName = TEXT("레이저 라이플");
    Item3.ItemDescription = TEXT("레이저 탄이 나가는 라이플 건입니다.");
    Item3.ItemType = TEXT("총기류");
    Item3.ItemPrice = 150;
    Item3.ItemDamage = 40;
    Item3.ItemImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Weapon/LaserRifle.LaserRifle")));
    Item3.ItemBlueprint = LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/Gun/BP_LaserRifle.BP_LaserRifle_C"));

    FItemData Item4;
    Item4.ItemName = TEXT("플라즈마 소드");
    Item4.ItemDescription = TEXT("칼날이 초고압 에너지 필드로 코팅되어 있어 일반적인 금속도 절단할 수 있는 플라즈마 소드입니다.");
    Item4.ItemType = TEXT("도검류");
    Item4.ItemPrice = 150;
    Item4.ItemDamage = 40;
    Item4.ItemImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Weapon/PlasmaSword.PlasmaSword")));
    Item4.ItemBlueprint = LoadClass<AActor>(nullptr, TEXT("/Game/Weapon/Sword/BP_PlasmaSword.BP_PlasmaSword_C"));


    ItemDataArray.Add(Item1);
    ItemDataArray.Add(Item2);
    ItemDataArray.Add(Item3);
    ItemDataArray.Add(Item4);
}

void UShopMenuWidget::CreateItemSlots()
{
    if (!ShopSlotClass || !ShopScrollBox)
    {
        return;
    }

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

void UShopMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

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

			UPopupWidget* Popup = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (Popup)
			{
				Popup->AddToViewport();
				Popup->InitializePopup(FormattedText, false);

				Popup->ConfirmClicked.AddDynamic(this, &UShopMenuWidget::OnPopupClose);
			}
		}
	}
	else
	{
		if (PopupWidgetClass)
		{
			UPopupWidget* Popup = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (Popup)
			{
                FText FormattedText = NSLOCTEXT("ShopMenu", "PurchaseConfirmation", "구매하시겠습니까?");

				Popup->AddToViewport();
				Popup->InitializePopup(FormattedText, true);

				Popup->ConfirmClicked.AddDynamic(this, &UShopMenuWidget::OnConfirmPurchase);
				Popup->CancelClicked.AddDynamic(this, &UShopMenuWidget::OnPopupClose);
			}
		}
	}
}

void UShopMenuWidget::OnConfirmPurchase()
{
	if (GameManager)
	{
		int32 CurrentEnergy = GameManager->GetEnergy();
		GameManager->SetEnergy(CurrentEnergy -SelectedItem.ItemPrice);

        GameManager->AddPurchasedItem(SelectedItem);

        ShowCurrentEnergy();
	}
}

void UShopMenuWidget::OnPopupClose()
{
	return;
}

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





