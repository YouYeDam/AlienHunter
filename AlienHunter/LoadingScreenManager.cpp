


#include "LoadingScreenManager.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


void ALoadingScreenManager::BeginPlay()
{
    Super::BeginPlay();

    // 로딩 위젯 생성 및 화면에 추가
    if (LoadingScreenClass)
    {
        LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport();
        }
    }

    UGameManager* GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameManager)
    {
        return;
    }

    FName CurrentMissionName = GameManager->GetCurrentMissionName();
    if (CurrentMissionName.IsNone())
    {
        return;
    }

    MissionName = CurrentMissionName;
    FString LevelPath = FString::Format(TEXT("/Game/Levels/Missions/{0}"), {MissionName.ToString()});

    // 비동기 로드 시작
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([this]()
    {
        // 로딩 완료 시 키 입력 대기
        bIsMissionReady = true;
        WaitForAnyKey();
    }));
}

void ALoadingScreenManager::WaitForAnyKey()
{
    if (!bIsMissionReady)
    {
        return;
    }

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        PlayerController->InputComponent->BindAction("AnyKey", IE_Pressed, this, &ALoadingScreenManager::OnKeyPressed);

        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }   
}

void ALoadingScreenManager::OnKeyPressed()
{
    UGameplayStatics::OpenLevel(GetWorld(), MissionName);

    if (LoadingWidget)
    {
        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;
    }
}
