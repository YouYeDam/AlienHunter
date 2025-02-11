


#include "HelpMenuWidget.h"
#include "MainPlayerController.h"
#include "PauseMenuManager.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HelpSlotWidget.h"
#include "Engine/DataTable.h"

void UHelpMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeInfoData();  // 도움말 정보 데이터 초기화

    if (ReturnToMenuButton)
    {
        ReturnToMenuButton->OnClicked.AddDynamic(this, &UHelpMenuWidget::OnMoveToReturnToMenuMenuClicked);
    }

    if (MoveTabButton)
    {
        MoveTabButton->OnClicked.AddDynamic(this, &UHelpMenuWidget::OnMoveTabClicked);
    }

    if (CombatTabButton)
    {
        CombatTabButton->OnClicked.AddDynamic(this, &UHelpMenuWidget::OnCombatTabClicked);
    }

    if (InteractTabButton)
    {
        InteractTabButton->OnClicked.AddDynamic(this, &UHelpMenuWidget::OnInteractTabClicked);
    }
}

// 도움말 정보 데이터를 초기화하는 함수
void UHelpMenuWidget::InitializeInfoData()
{
    if (!HelpInfoDataTable)
    {
        return;
    }
    static const FString MoveContextString(TEXT("Help Menu Move Info Initialization"));
    static const FString CombatContextString(TEXT("Help Menu Combat Info Initialization"));
    static const FString InteractContextString(TEXT("Help Menu Interact Info Initialization"));

    TArray<FHelpInfoData*> HelpInfos;
    HelpInfoDataTable->GetAllRows(TEXT(""), HelpInfos);

    for (FHelpInfoData* HelpInfo : HelpInfos)
    {
        if (HelpInfo)
        {
            if (HelpInfo->HelpInfoType == "Move")
            {
                MoveHelpInfoDataArray.Add(*HelpInfo);
            }
            else if (HelpInfo->HelpInfoType == "Combat")
            {
                CombatHelpInfoDataArray.Add(*HelpInfo);
            }
            else if (HelpInfo->HelpInfoType == "Interact")
            {
                InteractHelpInfoDataArray.Add(*HelpInfo);
            }
        }
    }
}

// 일시정지 메뉴로 돌아가는 메소드
void UHelpMenuWidget::OnMoveToReturnToMenuMenuClicked()
{
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());

    if (PlayerController)
    {
        APauseMenuManager* PauseMenuManager = PlayerController->GetPauseMenuManager();
        if (PauseMenuManager)
        {
            PauseMenuManager->ShowPauseMenu();
        }
    }
}

// 도움말 메뉴 항목을 "이동" 항목으로 전환하는 메소드
void UHelpMenuWidget::OnMoveTabClicked()
{
    CreateHelpSlots(MoveHelpInfoDataArray);
}

// 도움말 메뉴 항목을 "전투" 항목으로 전환하는 메소드
void UHelpMenuWidget::OnCombatTabClicked()
{
    CreateHelpSlots(CombatHelpInfoDataArray);
}

// 도움말 메뉴 항목을 "상호작용" 항목으로 전환하는 메소드
void UHelpMenuWidget::OnInteractTabClicked()
{
    CreateHelpSlots(InteractHelpInfoDataArray);
}

// 도움말 슬롯을 생성하고 초기화하는 메소드
void UHelpMenuWidget::CreateHelpSlots(const TArray<FHelpInfoData>& HelpInfoDataArray)
{
    if (!HelpScrollBox || !HelpSlotClass)
    {
        return;
    }

    HelpScrollBox->ClearChildren(); // 기존 슬롯 초기화

    // 슬롯 생성
    for (const FHelpInfoData& HelpInfoData : HelpInfoDataArray)
    {
        UHelpSlotWidget* HelpSlot = CreateWidget<UHelpSlotWidget>(this, HelpSlotClass);

        if (HelpSlot)
        {
            HelpSlot->InitializeSlot(this, HelpInfoData);
            HelpScrollBox->AddChild(HelpSlot);
        }
    }
}
