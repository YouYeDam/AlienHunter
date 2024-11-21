// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenuWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMenuGameMode.h"
#include "InventorySlotWidget.h"


void UInventoryMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        GameManager->SetInventoryMenuWidget(this);

        ItemDataArray = GameManager->GetPurchasedItems();
    }

    CreateInventorySlots();

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnMoveToGameMenuClicked);
    }

    if (EquipItemButton)
    {
        EquipItemButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnEquipItemClicked);
    }
}

void UInventoryMenuWidget::CreateInventorySlots()
{
    if (!InventorySlotClass || !InventoryScrollBox)
    {
        return;
    }

    for (const FItemData& Item : ItemDataArray)
    {
        UInventorySlotWidget* InventorySlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
        if (InventorySlot)
        {
            if (InventorySlot)
            {
                InventorySlot->InitializeSlot(this, Item);
            }

            InventoryScrollBox->AddChild(InventorySlot);
        }
    }
}

void UInventoryMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

void UInventoryMenuWidget::OnEquipItemClicked()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameManager || SelectedItem.ItemName.IsEmpty())
	{
		return;
	}
    if (SelectedItem.ItemType == TEXT("총기류")) 
    {
        GameManager->SetEquippedGunItemData(SelectedItem);
        GameManager->SetEquippedGun(SelectedItem.ItemBlueprint);
    }
    else if (SelectedItem.ItemType == TEXT("도검류")) 
    {
        GameManager->SetEquippedSwordItemData(SelectedItem);
        GameManager->SetEquippedSword(SelectedItem.ItemBlueprint);
    }
}

void UInventoryMenuWidget::UpdateItemDetails(const FItemData& ItemData)
{
    SelectedItem = ItemData;

    if (ItemName)
    {
        ItemName->SetText(FText::FromString(ItemData.ItemName));
    }
    if (ItemDescription)
    {
        ItemDescription->SetText(FText::FromString(ItemData.ItemDescription));
    }
    if (ItemDamage)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("InventoryMenu", "ItemDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        ItemDamage->SetText(FormattedText);
    }
    if (ItemImage && ItemData.ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

void UInventoryMenuWidget::UpdateGunDetails(const FItemData& ItemData)
{
    if (GunName)
    {
        GunName->SetText(FText::FromString(ItemData.ItemName));
    }
    if (GunDamage)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("InventoryMenu", "GunDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        GunDamage->SetText(FormattedText);
    }
    if (GunImage && ItemData.ItemImage)
    {
        GunImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

void UInventoryMenuWidget::UpdateSwordDetails(const FItemData& ItemData)
{
    if (SwordName)
    {
        SwordName->SetText(FText::FromString(ItemData.ItemName));
    }
    if (SwordDamage)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("InventoryMenu", "SwordDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        SwordDamage->SetText(FormattedText);
    }
    if (SwordImage && ItemData.ItemImage)
    {
        SwordImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

void UInventoryMenuWidget::UpdateItemDataArray()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        ItemDataArray = GameManager->GetPurchasedItems();
    }

    if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren();
    }

    CreateInventorySlots();
}
