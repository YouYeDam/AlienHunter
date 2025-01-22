


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoadGameButton)
    {
        LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameClicked);
    }

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
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
        OnStartGameClicked();
    }
}

// 새 게임을 시작하는 메소드
void UMainMenuWidget::OnStartGameClicked()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = false;
    }

    UGameplayStatics::OpenLevel(this, FName("GameMenu"));
}

// 게임을 종료하는 메소드
void UMainMenuWidget::OnQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}


