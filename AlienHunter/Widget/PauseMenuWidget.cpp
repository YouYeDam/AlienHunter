


#include "PauseMenuWidget.h"
#include "MainPlayerController.h"
#include "PauseMenuManager.h"
#include "Kismet/GameplayStatics.h"
#include "PopupWidget.h"
#include "AlienHunterGameMode.h"
#include "Components/Button.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HelpMenuButton)
    {
        HelpMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMoveToHelpMenuClicked);
    }
    
    if (SoundSettingButton)
    {
        SoundSettingButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSountSettingClicked);
    }

    if (MissionGiveupButton)
    {
        MissionGiveupButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMissionGiveupClicked);
    }

    if (ReturnToGameButton)
    {
        ReturnToGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnReturnToGameClicked);
    }
}

// 도움말 메뉴를 여는 메소드
void UPauseMenuWidget::OnMoveToHelpMenuClicked()
{
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());

    if (PlayerController)
    {
        APauseMenuManager* PauseMenuManager = PlayerController->GetPauseMenuManager();
        if (PauseMenuManager)
        {
            PauseMenuManager->ShowHelpMenu();
        }
    }
}

// 사운드 설정 메뉴를 여는 메소드
void UPauseMenuWidget::OnSountSettingClicked()
{
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());

    if (PlayerController)
    {
        APauseMenuManager* PauseMenuManager = PlayerController->GetPauseMenuManager();
        if (PauseMenuManager)
        {
            PauseMenuManager->ShowSoundMenu();
        }
    }
}

// 임무 포기 버튼 클릭 시 실행되는 메소드
void UPauseMenuWidget::OnMissionGiveupClicked()
{
    // 정말로 임무를 포기할 것인지 확인
    if (PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
        if (PopupWidget)
        {
            FText ConfirmMessage = NSLOCTEXT("PauseMenu", "MissionGiveupConfirmation", "임무 포기 시 실패로 간주됩니다.\n정말로 임무를 포기하겠습니까?");
            
            PopupWidget->InitializePopup(ConfirmMessage, true);

            PopupWidget->ConfirmClicked.AddDynamic(this, &UPauseMenuWidget::OnConfirmMissionGiveup);
            PopupWidget->CancelClicked.AddDynamic(this, &UPauseMenuWidget::OnPopupClose);

            PopupWidget->AddToViewport();
        }
    }
}

void UPauseMenuWidget::OnReturnToGameClicked()
{
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());

    if (PlayerController)
    {
        PlayerController->TogglePauseMenu();
    }
}


// 임무 포기 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UPauseMenuWidget::OnConfirmMissionGiveup()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UPauseMenuWidget::OnConfirmMissionGiveup);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UPauseMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }

    AAlienHunterGameMode* GameMode = Cast<AAlienHunterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->EndGame(false); // 게임 패배 처리 (임무 포기)
    }
}

// 임무 포기 버튼 클릭 후 취소 버튼 클릭 시 실행되는 메소드
void UPauseMenuWidget::OnPopupClose()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UPauseMenuWidget::OnConfirmMissionGiveup);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UPauseMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}
