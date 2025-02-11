


#include "PauseMenuManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HelpMenuWidget.h"

void APauseMenuManager::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// 일시정지 메뉴로 이동하는 메소드
void APauseMenuManager::ShowPauseMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
    }

    if (PauseMenuClass && PlayerController)
    {
        CurrentWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

// 도움말 메뉴로 이동하는 메소드
void APauseMenuManager::ShowHelpMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
    }

    if (HelpMenuClass && PlayerController)
    {
        CurrentWidget = CreateWidget<UUserWidget>(PlayerController, HelpMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
            UHelpMenuWidget* HelpMenuWidget = Cast<UHelpMenuWidget>(CurrentWidget);
            if (HelpMenuWidget)
            {
                HelpMenuWidget->OnMoveTabClicked();
            }
        }
    }
}

void APauseMenuManager::CloseAllMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
}


