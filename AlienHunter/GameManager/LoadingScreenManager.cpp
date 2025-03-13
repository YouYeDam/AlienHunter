


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

        // LoadingScreenWidget ShowMissionName을 호출해 이름 표시
		ULoadingScreenWidget* LoadingScreendWidget = Cast<ULoadingScreenWidget>(LoadingWidget);
        if (LoadingScreendWidget)
        {
            LoadingScreendWidget->ShowMissionName();
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

    FName CurrentMissionLevel = GameManager->GetCurrentMissionLevel();
    if (CurrentMissionLevel.IsNone())
    {
        return; 
    }

    MissionLevel = CurrentMissionLevel;

    FMissionData CurrentMissionData = GameManager->GetCurrentMissionData(); // 현재 미션 데이터 받아오기
    LoadingScreen->UpdateBackground(CurrentMissionData.MissionImage); // 로딩 스크린 배경 설정

    LoadingScreen->UpdateLoadingText(false); // 로딩 진행 상태 텍스트를 로딩 중으로 설정

    // 비동기 로드 시작
    FString LevelPath = FString::Format(TEXT("/Game/Levels/Missions/{0}"), {MissionLevel.ToString()});
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([this, LoadingScreen]()
    {
        // 로딩 완료 처리
        bIsMissionReady = true;
        WaitForAnyKey();

        if (LoadingScreen)
        {
            LoadingScreen->UpdateLoadingText(true); // 로딩 진행 상태 텍스트를 로딩 완료로 설정
        }
    }));
}

// 아무 키 입력을 받을 수 있도록 설정하는 메소드
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

// 키 입력을 감지한 후, 레벨을 로드하는 메소드
void ALoadingScreenManager::OnKeyPressed()
{
    if (LoadingWidget)
    {
        LoadingWidget->RemoveFromParent();
        LoadingWidget = nullptr;
    }

    UGameplayStatics::OpenLevel(GetWorld(), MissionLevel);
}
