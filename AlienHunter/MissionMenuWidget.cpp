


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
    FMissionData Mission1;
    Mission1.MissionName = TEXT("임무명 : 숲의 안전 확보하기");
    Mission1.MissionObject = TEXT("임무 목표 : 모든 적 섬멸");
    Mission1.MissionDescription = TEXT("인근 숲에 외계인 세력이 등장했다는 보고가 들어왔습니다. 난폭해진 동물들과 외계인을 제압하세요.");
    Mission1.MissionLevel = TEXT("Mission1");
    Mission1.MissionImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Background/Forest.Forest")));
    Mission1.MissionEnergyReward = 300;
    Mission1.MissionEXPReward = 100;
    Mission1.MissionID = 1;

    FMissionData Mission2;
    Mission2.MissionName = TEXT("임무명 : 쓰레기장의 안전 확보하기");
    Mission2.MissionObject = TEXT("임무 목표 : 모든 적 섬멸");
    Mission2.MissionDescription = TEXT("쓰레기장에 외계인 세력이 숨어 공격을 준비한다는 보고가 들어왔습니다. 이들에게 본때를 보여주세요.");
    Mission2.MissionLevel = TEXT("Mission2");
    Mission2.MissionImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Background/Junkyard.Junkyard")));
    Mission2.MissionEnergyReward = 500;
    Mission2.MissionEXPReward = 200;
    Mission2.MissionID = 2;

    MissionDataArray.Add(Mission1);
    MissionDataArray.Add(Mission2);
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
        GameManager->SetCurrentMissionData(SelectedMissionData); // 선택한 미션을 게임 매니저의 현재 미션으로 설정
    }

    // 미션 진입 시 플레이어 인풋 활성화 및 마우스 커서 비활성화 메소드
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = false;
    }

    SelectedMissionData = FMissionData(); // 선택된 미션 비우기
    UGameplayStatics::OpenLevel(this, SelectedMissionLevel); // 선택된 미션 레벨 로드
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