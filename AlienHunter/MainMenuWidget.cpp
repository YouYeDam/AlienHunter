


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "GameManager.h"
#include "PopupWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoadGameButton)
    {
        LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameClicked);
    }

    if (StartNewGameButton)
    {
        StartNewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartNewGameClicked);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);
    }
}

// 게임을 로드하는 메소드
void UMainMenuWidget::OnLoadGameClicked()
{
    UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameManager)
    {
        GameManager->LoadGame();
        GameStart();
    }
}

// 새 게임을 진행하는 메소드
void UMainMenuWidget::OnStartNewGameClicked()
{
    if (PopupWidgetClass)
    {
        PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
        if (PopupWidget)
        {
            FText FormattedText = NSLOCTEXT("MainMenu", "NewGameConfirmation", "기존에 저장된 게임은 삭제됩니다.\n새 게임을 시작하시겠습니까?");

            PopupWidget->AddToViewport();
            PopupWidget->InitializePopup(FormattedText, true);

            PopupWidget->ConfirmClicked.AddDynamic(this, &UMainMenuWidget::OnConfirmNewGame);
            PopupWidget->CancelClicked.AddDynamic(this, &UMainMenuWidget::OnPopupClose);
        }
    }
}

// 새 게임 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UMainMenuWidget::OnConfirmNewGame()
{
    UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameManager)
    {
        GameManager->DeleteGame();
        GameStart();
        GameManager->SaveGame();
    }

    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UMainMenuWidget::OnConfirmNewGame);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UMainMenuWidget::OnPopupClose);
        PopupWidget = nullptr;
    }
}

// 새 게임 버튼 클릭 후 취소 버튼 클릭 시 실행되는 메소드
void UMainMenuWidget::OnPopupClose()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UMainMenuWidget::OnConfirmNewGame);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UMainMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}

// 게임을 종료하는 메소드
void UMainMenuWidget::OnQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

// 게임을 시작하는 메소드
void UMainMenuWidget::GameStart()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = false;
    }

    UGameplayStatics::OpenLevel(this, FName("GameMenu"));
}