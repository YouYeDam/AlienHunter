


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

    // 미션 데이터 초기화
    InitializeMissionData();

    // 슬롯 동적 생성
    CreateMissionSlots();

    if (GameMenuButton)
    {
        GameMenuButton->OnClicked.AddDynamic(this, &UMissionMenuWidget::OnMoveToGameMenuClicked);
    }

    if (StartMissionButton)
    {
        StartMissionButton->OnClicked.AddDynamic(this, &UMissionMenuWidget::OnStartMissionClicked);
    }
}

void UMissionMenuWidget::InitializeMissionData()
{
    FMissionData Mission1;
    Mission1.MissionName = TEXT("숲의 안전 확보하기");
    Mission1.MissionObject = TEXT("임무 목표 : 모든 적 섬멸");
    Mission1.MissionDescription = TEXT("인근 숲에 외계인 세력이 등장했다는 보고가 들어왔습니다. 난폭해진 동물들과 외계인을 제압하세요.");
    Mission1.MissionLevel = TEXT("Mission1");
    Mission1.MissionImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Image/Background/Forest.Forest")));
    Mission1.MissionEnergyReward = 300;
    Mission1.MissionEXPReward = 100;
    Mission1.MissionID = 1;

    FMissionData Mission2;
    Mission2.MissionName = TEXT("쓰레기장의 안전 확보하기");
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

void UMissionMenuWidget::CreateMissionSlots()
{
    if (!MissionSlotClass || !MissionScrollBox) 
    {
        return;
    }

    // 모든 미션 데이터를 기반으로 슬롯 생성
    for (const FMissionData& Mission : MissionDataArray)
    {
        // 슬롯 생성
        UMissionSlotWidget* MissionSlot = CreateWidget<UMissionSlotWidget>(this, MissionSlotClass);
        if (MissionSlot)
        {
            // 슬롯 초기화
            MissionSlot->InitializeSlot(this, Mission);

            // 슬롯을 스크롤 박스에 추가
            MissionScrollBox->AddChild(MissionSlot);
        }
    }
}

void UMissionMenuWidget::OnMoveToGameMenuClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowGameMenu();
    }
}

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
    }
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = false;
    }

    SelectedMissionData = FMissionData();
    UGameplayStatics::OpenLevel(this, SelectedMissionLevel);
}

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