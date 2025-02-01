


#include "GameMenuWidget.h"
#include "Components/Button.h"
#include "GameManager.h"
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

    if (SaveGameButton)
    {
        SaveGameButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnSaveGameClicked);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnQuitGameClicked);
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

// 게임 저장을 요청하는 메소드
void UGameMenuWidget::OnSaveGameClicked()
{
    UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameManager)
    {
        bool bSaveSuccessful = GameManager->SaveGame();

        if (PopupWidgetClass)
        {
            PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
            if (PopupWidget)
            {
                FText FormattedText;
                if (bSaveSuccessful)
                {
                    FormattedText = NSLOCTEXT("GameMenu", "SaveSuccessful", "게임이 성공적으로 저장되었습니다!");
                }
                else
                {
                    FormattedText = NSLOCTEXT("GameMenu", "SaveFailed", "게임 저장에 실패했습니다.");
                }

                PopupWidget->AddToViewport();
                PopupWidget->InitializePopup(FormattedText, false);

                PopupWidget->ConfirmClicked.AddDynamic(this, &UGameMenuWidget::OnPopupClose);
            }
        }
    }
}

// 게임을 종료하는 메소드
void UGameMenuWidget::OnQuitGameClicked()
{
    if (PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
        if (PopupWidget)
        {
            FText FormattedText = NSLOCTEXT("GameMenu", "QuitGameConfirmation", "정말 게임을 종료하시겠습니까?\n게임은 자동 저장됩니다.");

            PopupWidget->AddToViewport();
            PopupWidget->InitializePopup(FormattedText, true);

            PopupWidget->ConfirmClicked.AddDynamic(this, &UGameMenuWidget::OnConfirmQuitGame);
            PopupWidget->CancelClicked.AddDynamic(this, &UGameMenuWidget::OnPopupClose);
        }
    }
}

// 게임 종료 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UGameMenuWidget::OnConfirmQuitGame()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UGameMenuWidget::OnConfirmQuitGame);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UGameMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
    OnSaveGameClicked();
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

// 게임 종료 버튼 클릭 후 취소 버튼 클릭 시 실행되는 메소드
void UGameMenuWidget::OnPopupClose()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UGameMenuWidget::OnConfirmQuitGame);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UGameMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}
