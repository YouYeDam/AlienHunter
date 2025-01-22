


#include "MissionMenuWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMenuGameMode.h"
#include "MissionSlotWidget.h"

void UMissionMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    
    InitializeMissionData(); // 미션 데이터 초기화

    CreateMissionSlots(); // 미션 슬롯 초기화

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UMissionMenuWidget::OnMoveToGameMenuClicked);
    }

    if (StartMissionButton)
    {
        StartMissionButton->OnClicked.AddDynamic(this, &UMissionMenuWidget::OnStartMissionClicked);
    }
}

// 미션 데이터를 초기화하는 함수(json 파일 혹은 블루프린트 데이터 에셋을 이용한 방법으로 전환하는 것 고려)
void UMissionMenuWidget::InitializeMissionData()
{
    if (!MissionDataTable)
    {
        return;
    }

    static const FString ContextString(TEXT("Mission Menu Mission Initialization"));
    TArray<FMissionData*> Missions;

    // 데이터 테이블에서 모든 행 가져오기
    MissionDataTable->GetAllRows(ContextString, Missions);

    for (FMissionData* Mission : Missions)
    {
        if (Mission)
        {
            MissionDataArray.Add(*Mission); // 배열에 미션 추가
        }
    }
}

// 미션 슬롯을 생성하고 초기화하는 메소드
void UMissionMenuWidget::CreateMissionSlots()
{
    if (!MissionSlotClass || !MissionScrollBox) 
    {
        return;
    }

    // 미션 데이터를 순회하며 슬롯을 생성하고 초기화
    for (const FMissionData& Mission : MissionDataArray)
    {
        UMissionSlotWidget* MissionSlot = CreateWidget<UMissionSlotWidget>(this, MissionSlotClass);
        if (MissionSlot)
        {
            MissionSlot->InitializeSlot(this, Mission);

            MissionScrollBox->AddChild(MissionSlot);
        }
    }
}

// 메인 메뉴로 이동하는 메소드
void UMissionMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

// 선택한 미션으로 게임을 시작하는 메소드
void UMissionMenuWidget::OnStartMissionClicked()
{
    if (SelectedMissionData.MissionName.IsEmpty())
    {
        return;
    }

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameManager)
    {
        GameManager->SetCurrentMissionData(SelectedMissionData);
        GameManager->SetCurrentMissionName(SelectedMissionLevel);
        GameManager->SaveGame(); // 게임 저장
        UGameplayStatics::OpenLevel(this, FName("LoadingScreen")); // 로딩 화면 레벨로 이동
    }

    // UI 상태 초기화
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }

    SelectedMissionData = FMissionData(); // 선택된 미션 비우기
}

// 선택된 미션과 세부사항 UI를 업데이트하는 메소드
void UMissionMenuWidget::UpdateMissionDetails(const FMissionData& MissionData)
{
    SelectedMissionData = MissionData;
    
    if (MissionName)
    {
        MissionName->SetText(FText::FromString(MissionData.MissionName));
    }
    if (MissionObject)
    {
        MissionObject->SetText(FText::FromString(MissionData.MissionObject));
    }
    if (MissionReward)
    {
        FText FormattedText = FText::Format(
            NSLOCTEXT("MissionMenu", "MissionReward", "미션보상: {0}에너지 {1}경험치"),
            FText::AsNumber(MissionData.MissionEnergyReward),
            FText::AsNumber(MissionData.MissionEXPReward)
        );
        MissionReward->SetText(FormattedText);
    }
    if (MissionDescription)
    {
        MissionDescription->SetText(FText::FromString(MissionData.MissionDescription));
    }
    if (!MissionData.MissionLevel.IsEmpty())
    {
        SelectedMissionLevel = FName(*MissionData.MissionLevel);
    }
    if (MissionImage && MissionData.MissionImage)
    {
        MissionImage->SetBrushFromTexture(MissionData.MissionImage);
    }
}