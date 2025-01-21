


#include "LoadingScreenManager.h"
#include "GameManager.h"
#include "LoadingScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void ALoadingScreenManager::BeginPlay()
{
    Super::BeginPlay();

    // 로딩 위젯 생성 및 화면에 추가
    ULoadingScreenWidget* LoadingScreen = nullptr;
    if (LoadingScreenClass)
    {
        LoadingWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingScreenClass);
        if (LoadingWidget)
        {
            LoadingWidget->AddToViewport();
        }
    }

    LoadingScreen = Cast<ULoadingScreenWidget>(LoadingWidget);
    if (!LoadingScreen)
    {
        return;
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

    FMissionData CurrentMissionData = GameManager->GetCurrentMissionData();
    LoadingScreen->UpdateBackground(CurrentMissionData.MissionImage);

    LoadingScreen->UpdateLoadingText(false);

    // 비동기 로드 시작
    FString LevelPath = FString::Format(TEXT("/Game/Levels/Missions/{0}"), {MissionName.ToString()});
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([this, LoadingScreen]()
    {
        // 로딩 완료 처리
        UE_LOG(LogTemp, Warning, TEXT("로딩 완료"));

        bIsMissionReady = true;
        WaitForAnyKey();

        if (LoadingScreen)
        {
            LoadingScreen->UpdateLoadingText(true);
        }
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
    if (LoadingWidget)
    {
        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;
    }

    UGameplayStatics::OpenLevel(GetWorld(), MissionName);
}
