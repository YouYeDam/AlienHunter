


#include "GameMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (MissionMenuButton)
    {
        MissionMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToMissionMenuClicked);
    }

    if (ShopMenuButton)
    {
        ShopMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToShopMenuClicked);
    }

    if (InventoryMenuButton)
    {
        InventoryMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToInventoryMenuClicked);
    }

    if (CharacterMenuButton)
    {
        CharacterMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToCharacterMenuClicked);
    }
}

// 미션 메뉴로 이동하는 메소드
void UGameMenuWidget::OnMoveToMissionMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowMissionMenu();
    }
}

// 상점 메뉴로 이동하는 메소드
void UGameMenuWidget::OnMoveToShopMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowShopMenu();
    }
}

// 인벤토리 메뉴로 이동하는 메소드
void UGameMenuWidget::OnMoveToInventoryMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowInventoryMenu();
    }
}

// 캐릭터 메뉴로 이동하는 메소드
void UGameMenuWidget::OnMoveToCharacterMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowCharacterMenu();
    }
}


