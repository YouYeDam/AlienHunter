


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

        ItemDataArray = GameManager->GetPurchasedItems(); // 아이템 데이터에 구매한 아이템 모두 가져오기
    }

    CreateInventorySlots(); // 인벤토리 슬롯 초기화

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnMoveToGameMenuClicked);
    }

    if (EquipItemButton)
    {
        EquipItemButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnEquipItemClicked);
    }
}

// 인벤토리 슬롯을 생성하고 초기화하는 메소드
void UInventoryMenuWidget::CreateInventorySlots()
{
    if (!InventorySlotClass || !InventoryScrollBox)
    {
        return;
    }

    // 인벤토리 아이템 데이터를 순회하며 슬롯을 생성하고 초기화
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

// 게임 메뉴로 이동하는 메소드
void UInventoryMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

// 인벤토리의 아이템을 장착하는 메소드
void UInventoryMenuWidget::OnEquipItemClicked()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameManager || SelectedItem.ItemName.IsEmpty())
	{
		return;
	}

    // 게임 매니저에서 장착중인 무기의 아이템 데이터와 액터 바꿔주기
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

// 선택된 아이템과 세부사항 UI를 업데이트하는 메소드
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

// 장착중인 총기류의 UI를 업데이트하는 메소드
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

// 장착중인 도검류의 UI를 업데이트하는 메소드
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

// 인벤토리 아이템을 업데이트하는 메소드
void UInventoryMenuWidget::UpdateItemDataArray()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        ItemDataArray = GameManager->GetPurchasedItems();
    }

    if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren(); // 인벤토리 슬롯 생성 전 모두 비우기
    }

    CreateInventorySlots(); // 인벤토리 슬롯 재생성
}
