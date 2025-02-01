

#include "MissionSummaryManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MissionSummaryWidget.h"



void AMissionSummaryManager::BeginPlay()
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

	if (MissionSummaryClass)
    {
        MissionSummaryWidget = CreateWidget<UMissionSummaryWidget>(GetWorld(), MissionSummaryClass);
        if (MissionSummaryWidget)
        {
            MissionSummaryWidget->AddToViewport();
        }

		// MissionSummaryWidget의 ShowResult를 호출해 결과 표시
		UMissionSummaryWidget* SummaryWidget = Cast<UMissionSummaryWidget>(MissionSummaryWidget);
        if (SummaryWidget)
        {
            SummaryWidget->ShowResult();
        }
    }
}
