


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

         // 아이템 데이터에 구매한 아이템 모두 가져오기
        GunItemDataArray = GameManager->GetPurchasedGunItems();
        SwordItemDataArray = GameManager->GetPurchasedSwordItems();
        GrenadeItemDataArray = GameManager->GetPurchasedGrenadeItems();
    }

    CreateGunInventorySlots(); // 총기류 인벤토리 슬롯 초기화(처음에는 총기류가 선택되기 때문)

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnMoveToGameMenuClicked);
    }

    if (EquipItemButton)
    {
        EquipItemButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnEquipItemClicked);
    }

    if (GunTabButton)
    {
        GunTabButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnGunTabClicked);
    }

    if (SwordTabButton)
    {
        SwordTabButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnSwordTabClicked);
    }

    if (GrenadeTabButton)
    {
        GrenadeTabButton->OnClicked.AddDynamic(this, &UInventoryMenuWidget::OnGrenadeTabClicked);
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
        GameManager->SetEquippedGunItemData(SelectedGunItem);
        GameManager->SetEquippedGun(SelectedItem.ItemBlueprint);
    }
    else if (SelectedItem.ItemType == TEXT("도검류")) 
    {
        GameManager->SetEquippedSwordItemData(SelectedSwordItem);
        GameManager->SetEquippedSword(SelectedItem.ItemBlueprint);
    }
    else if (SelectedItem.ItemType == TEXT("수류탄류")) 
    {
        GameManager->SetEquippedGrenadeItemData(SelectedGrenadeItem);
        GameManager->SetEquippedGrenade(SelectedItem.ItemBlueprint);
    }
}

// 총기류 슬롯을 생성하는 메소드
void UInventoryMenuWidget::OnGunTabClicked()
{
    CreateGunInventorySlots();
}

// 도검류 슬롯을 생성하는 메소드
void UInventoryMenuWidget::OnSwordTabClicked()
{
    CreateSwordInventorySlots();
}

// 수류탄류 슬롯을 생성하는 메소드
void UInventoryMenuWidget::OnGrenadeTabClicked()
{
    CreateGrenadeInventorySlots();
}

// 총기류 인벤토리 슬롯을 생성하고 초기화하는 메소드
void UInventoryMenuWidget::CreateGunInventorySlots()
{
    if (!InventorySlotClass || !InventoryScrollBox)
    {
        return;
    }

    InventoryScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 총기류 슬롯 생성
    for (const FGunItemData& GunItem : GunItemDataArray)
    {
        UInventorySlotWidget* GunSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
        if (GunSlot)
        {
            GunSlot->InitializeGunSlot(this, GunItem);
            InventoryScrollBox->AddChild(GunSlot);
        }
    }
}

// 도검류 인벤토리 슬롯을 생성하고 초기화하는 메소드
void UInventoryMenuWidget::CreateSwordInventorySlots()
{
    if (!InventorySlotClass || !InventoryScrollBox)
    {
        return;
    }

    InventoryScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 도검류 슬롯 생성
    for (const FSwordItemData& SwordItem : SwordItemDataArray)
    {
        UInventorySlotWidget* SwordSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
        if (SwordSlot)
        {
            SwordSlot->InitializeSwordSlot(this, SwordItem);
            InventoryScrollBox->AddChild(SwordSlot);
        }
    }
}

// 수류탄류 인벤토리 슬롯을 생성하고 초기화하는 메소드
void UInventoryMenuWidget::CreateGrenadeInventorySlots()
{
    if (!InventorySlotClass || !InventoryScrollBox)
    {
        return;
    }

    InventoryScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 수류탄류 슬롯 생성
    for (const FGrenadeItemData& GrenadeItem : GrenadeItemDataArray)
    {
        UInventorySlotWidget* GrenadeSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
        if (GrenadeSlot)
        {
            GrenadeSlot->InitializeGrenadeSlot(this, GrenadeItem);
            InventoryScrollBox->AddChild(GrenadeSlot);
        }
    }
}

// 선택된 아이템과 세부사항 UI를 업데이트하는 메소드
void UInventoryMenuWidget::UpdateItemDetails(const FBaseItemData& ItemData)
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
void UInventoryMenuWidget::UpdateGunDetails(const FGunItemData& ItemData)
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
void UInventoryMenuWidget::UpdateSwordDetails(const FSwordItemData& ItemData)
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

// 장착중인 수류탄류의 UI를 업데이트하는 메소드
void UInventoryMenuWidget::UpdateGrenadeDetails(const FGrenadeItemData& ItemData)
{
    if (GrenadeName)
    {
        GrenadeName->SetText(FText::FromString(ItemData.ItemName));
    }
    if (GrenadeDamage)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("InventoryMenu", "GrenadeDamageText", "공격력: {0}"),
            FText::AsNumber(ItemData.ItemDamage)
        );
        GrenadeDamage->SetText(FormattedText);
    }
    if (GrenadeImage && ItemData.ItemImage)
    {
        GrenadeImage->SetBrushFromTexture(ItemData.ItemImage);
    }
}

// 인벤토리 총기류 아이템을 업데이트하는 메소드
void UInventoryMenuWidget::UpdateGunItemDataArray()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        GunItemDataArray = GameManager->GetPurchasedGunItems();
    }

    if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren(); // 인벤토리 슬롯 생성 전 모두 비우기
    }
}

// 인벤토리 도검류 아이템을 업데이트하는 메소드
void UInventoryMenuWidget::UpdateSwordItemDataArray()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        SwordItemDataArray = GameManager->GetPurchasedSwordItems();
    }

    if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren(); // 인벤토리 슬롯 생성 전 모두 비우기
    }
}

// 인벤토리 수류탄류 아이템을 업데이트하는 메소드
void UInventoryMenuWidget::UpdateGrenadeItemDataArray()
{
    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (GameManager) 
    {
        GrenadeItemDataArray = GameManager->GetPurchasedGrenadeItems();
    }

    if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren(); // 인벤토리 슬롯 생성 전 모두 비우기
    }
}

// 빈 데이터로 초기화하는 메소드
void UInventoryMenuWidget::ResetSelectedItems()
{
    SelectedGunItem = FGunItemData();
    SelectedSwordItem = FSwordItemData();
}

void UInventoryMenuWidget::SetSelectedGunItem(const FGunItemData& GunItem)
{
    SelectedGunItem = GunItem;
}

void UInventoryMenuWidget::SetSelectedSwordItem(const FSwordItemData& SwordItem)
{
    SelectedSwordItem = SwordItem;
}

void UInventoryMenuWidget::SetSelectedGrenadeItem(const FGrenadeItemData& GrenadeItem)
{
    SelectedGrenadeItem = GrenadeItem;
}