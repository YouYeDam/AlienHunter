


#include "PerkMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PopupWidget.h"
#include "TooltipWidget.h"
#include "PerkSlotWidget.h"
#include "GameMenuGameMode.h"

void UPerkMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializePerkData(); // 퍽 데이터 초기화

    CreatePerkSlots(); // 퍽 슬롯 초기화

    UpdatePerkSlots();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if (CharacterMenuButton)
    {
        CharacterMenuButton->OnClicked.AddDynamic(this, &UPerkMenuWidget::OnMoveToCharacterMenuButtonClicked);
    }

    if (PerkResetButton)
    {
        PerkResetButton->OnClicked.AddDynamic(this, &UPerkMenuWidget::OnPerkResetButtonClicked);
    }
}

// 퍽 데이터를 초기화하는 메소드
void UPerkMenuWidget::InitializePerkData()
{
    if (!PerkDataTable)
    {
        return;
    }

    static const FString ContextString(TEXT("Perk Menu Perk Initialization"));
    TArray<FPerkData*> Perks;

    // 데이터 테이블에서 모든 행 가져오기
    PerkDataTable->GetAllRows(ContextString, Perks);

    for (FPerkData* Perk : Perks)
    {
        if (Perk)
        {
            PerkDataArray.Add(*Perk); // 배열에 퍽 추가
        }
    }
}

// 퍽 슬롯을 생성하는 메소드
void UPerkMenuWidget::CreatePerkSlots()
{
    if (!PerkSlotClass) return;

    // 레벨별 퍽 박스 매핑
    TMap<int32, UHorizontalBox*> PerkBoxMap;
    PerkBoxMap.Add(1, PerkBoxLV1);
    PerkBoxMap.Add(5, PerkBoxLV5);
    PerkBoxMap.Add(10, PerkBoxLV10);
    PerkBoxMap.Add(15, PerkBoxLV15);
    PerkBoxMap.Add(20, PerkBoxLV20);

    // 퍽 박스에 각 레벨에 맞는 퍽 슬롯을 추가
    for (const FPerkData& Perk : PerkDataArray)
    {
        UHorizontalBox** FoundBox = PerkBoxMap.Find(Perk.PerkRequiredLevel);
        if (FoundBox && *FoundBox)
        {
            UPerkSlotWidget* PerkSlot = CreateWidget<UPerkSlotWidget>(this, PerkSlotClass);
            if (PerkSlot)
            {
                PerkSlot->InitializeSlot(this, Perk);
                (*FoundBox)->AddChild(PerkSlot);
            }
        }
    }
}

// 캐릭터 메뉴로 이동하는 메소드
void UPerkMenuWidget::OnMoveToCharacterMenuButtonClicked()
{
    AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ShowCharacterMenu();
    }
}

// 퍽 초기화 버튼 클릭 시 실행되는 메소드
void UPerkMenuWidget::OnPerkResetButtonClicked()
{
    if (!GameManager)
    {
        return;
    }

    int32 RequiredEnergy = GameManager->GetPlayerLevel() * 100; // 초기화에 필요한 에너지

    const TArray<FPerkData>& ChosenPerks = GameManager->GetChosenPerks();
    if (ChosenPerks.IsEmpty())
    {
        if (PopupWidgetClass)
        {
            FText FormattedText = NSLOCTEXT("PerkMenu", "AlreadyResetPerks", "이미 퍽이 초기화 된 상태입니다.");

            PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
            if (PopupWidget)
            {
                PopupWidget->AddToViewport();
                PopupWidget->InitializePopup(FormattedText, false);

                PopupWidget->ConfirmClicked.AddDynamic(this, &UPerkMenuWidget::OnPopupClose);
            }
        }
    }
    else if (GameManager->GetEnergy() < RequiredEnergy) // 현재 에너지가 초기화에 필요한 에너지보다 적다면 실패
    {
    	if (PopupWidgetClass)
		{
            FText FormattedText = NSLOCTEXT("PerkMenu", "InsufficientEnergy", "에너지가 부족합니다!");

			PopupWidget  = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (PopupWidget)
			{
				PopupWidget->AddToViewport();
				PopupWidget->InitializePopup(FormattedText, false);

				PopupWidget->ConfirmClicked.AddDynamic(this, &UPerkMenuWidget::OnPopupClose);
			}
		}
    }
    else
    {
		if (PopupWidgetClass)
		{
			PopupWidget = CreateWidget<UPopupWidget>(this, PopupWidgetClass);
			if (PopupWidget)
			{
                FText FormattedText = FText::Format(NSLOCTEXT("ShopMenu", "PurchaseConfirmation", "초기화에 필요한 비용은 {0} 에너지입니다.\n 초기화를 진행하겠습니까?"), 
                                                    FText::AsNumber(RequiredEnergy));

				PopupWidget->AddToViewport();
				PopupWidget->InitializePopup(FormattedText, true);

				PopupWidget->ConfirmClicked.AddDynamic(this, &UPerkMenuWidget::OnConfirmResetPerks);
				PopupWidget->CancelClicked.AddDynamic(this, &UPerkMenuWidget::OnPopupClose);
			}
		}
    }
}

// 퍽 초기화 버튼 클릭 후 확인 버튼 클릭 시 실행되는 메소드
void UPerkMenuWidget::OnConfirmResetPerks()
{
    int32 RequiredEnergy = GameManager->GetPlayerLevel() * 100;

    GameManager->SetEnergy(GameManager->GetEnergy() - RequiredEnergy);
    GameManager->ClearShosenPerks();
    
    UpdatePerkSlots();
    
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UPerkMenuWidget::OnConfirmResetPerks);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UPerkMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}

// 퍽 초기화 버튼 클릭 후 취소 버튼 클릭 시 실행되는 메소드
void UPerkMenuWidget::OnPopupClose()
{
    if (PopupWidget)
    {
        PopupWidget->ConfirmClicked.RemoveDynamic(this, &UPerkMenuWidget::OnConfirmResetPerks);
        PopupWidget->CancelClicked.RemoveDynamic(this, &UPerkMenuWidget::OnPopupClose);
        PopupWidget->RemoveFromParent();
        PopupWidget = nullptr;
    }
}

// 퍽 슬롯 상태를 업데이트하는 메소드
void UPerkMenuWidget::UpdatePerkSlots()
{
    TArray<UPerkSlotWidget*> AllPerkSlots;
    
    for (UHorizontalBox* PerkBox : { PerkBoxLV1, PerkBoxLV5, PerkBoxLV10, PerkBoxLV15, PerkBoxLV20 })
    {
        if (!PerkBox)
        {
            continue;
        }

        for (UWidget* ChildWidget : PerkBox->GetAllChildren())
        {
            UPerkSlotWidget* PerkSlot = Cast<UPerkSlotWidget>(ChildWidget);
            if (PerkSlot)
            {
                PerkSlot->UpdateSlotColor();
            }
        }
    }
}

// 퍽에 마우스 오버 시 툴팁을 보여주는 메소드
void UPerkMenuWidget::ShowTooltip(const FText& Title, const FText& Description, FVector2D MousePosition)
{
    if (!TooltipWidgetClass)
    {
        return;
    }

    if (!PerkTooltipWidget)
    {
        PerkTooltipWidget = CreateWidget<UTooltipWidget>(GetWorld(), TooltipWidgetClass);
        if (PerkTooltipWidget)
        {
            PerkTooltipWidget->AddToViewport();
        }
    }

    PerkTooltipWidget->SetTooltipData(Title, Description);

    PerkTooltipWidget->SetPositionInViewport(MousePosition + FVector2D(5.0f, 5.0f));
}

// 퍽에 마우스 오버 시 툴팁을 감추는 메소드
void UPerkMenuWidget::HideTooltip()
{
    if (PerkTooltipWidget)
    {
        PerkTooltipWidget->RemoveFromParent();
        PerkTooltipWidget = nullptr;
    }
}