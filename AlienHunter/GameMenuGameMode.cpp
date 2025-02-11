


#include "GameMenuGameMode.h"
#include "ShopMenuWidget.h"
#include "InventoryMenuWidget.h"
#include "CharacterMenuWidget.h"
#include "PerkMenuWidget.h"
#include "Blueprint/UserWidget.h"

// 게임 메뉴 진입 시 플레이어 인풋 활성화 및 마우스 커서 활성화 메소드
void AGameMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }

    // 초기 메뉴 표시
    ShowGameMenu();
}

// 게임 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowGameMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (GameMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

// 미션 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowMissionMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (MissionMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MissionMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

// 상점 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowShopMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (ShopMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ShopMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UShopMenuWidget* ShopMenuWidget = Cast<UShopMenuWidget>(CurrentWidget);
            if (ShopMenuWidget)
            {
                ShopMenuWidget->ShowCurrentEnergy();
            }
        }
    }
}

// 인벤토리 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowInventoryMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (InventoryMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UInventoryMenuWidget* InventoryMenuWidget = Cast<UInventoryMenuWidget>(CurrentWidget);
            if (InventoryMenuWidget)
            {
                InventoryMenuWidget->CreateGunInventorySlots();
            }
        }
    }
}

// 캐릭터 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowCharacterMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (CharacterMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), CharacterMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UCharacterMenuWidget* CharacterMenuWidget = Cast<UCharacterMenuWidget>(CurrentWidget);
            if (CharacterMenuWidget)
            {
                CharacterMenuWidget->ShowPlayerStat();
            }
        }
    }
}

// 퍽 메뉴로 이동하는 메소드
void AGameMenuGameMode::ShowPerkMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (PerkMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PerkMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UPerkMenuWidget* PerkMenuWidget = Cast<UPerkMenuWidget>(CurrentWidget);
            if (PerkMenuWidget)
            {
                PerkMenuWidget->UpdatePerkSlots();
            }
        }
    }
}