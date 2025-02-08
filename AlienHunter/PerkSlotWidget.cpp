


#include "PerkSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UPerkSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UPerkSlotWidget::OnSlotClicked);
        SlotButton->OnHovered.AddDynamic(this, &UPerkSlotWidget::OnHovered);
        SlotButton->OnUnhovered.AddDynamic(this, &UPerkSlotWidget::OnUnhovered);
    }

    UpdateSlotColor();
}

// 퍽 슬롯을 설정하는 메소드
void UPerkSlotWidget::InitializeSlot(UPerkMenuWidget* InPerkMenuWidget, const FPerkData& InPerkData)
{
    PerkMenuWidgetRef = InPerkMenuWidget;
    PerkData = InPerkData;

    if (PerkNameText)
    {
        PerkNameText->SetText(FText::FromString(PerkData.PerkName));
    }
}

// 슬롯 클릭 시 게임 매니저에 퍽을 추가하는 메소드
void UPerkSlotWidget::OnSlotClicked()
{
    if (!GameManager)
    {
        return;
    }

    const TArray<FPerkData>& ChosenPerks = GameManager->GetChosenPerks();

    // 같은 퍽이 이미 선택되어 있는지 확인
    for (const FPerkData& ExistingPerk : ChosenPerks)
    {
        if (ExistingPerk.PerkType == PerkData.PerkType)  // 퍽이 동일한 경우
        {
            return;
        }

        if (ExistingPerk.PerkRequiredLevel == PerkData.PerkRequiredLevel)  // 같은 레벨의 퍽이 이미 선택된 경우
        {
            return;
        }
    }

    
    GameManager->AddChosenPerks(PerkData); // 퍽 추가

    UpdateSlotColor(); // 버튼 색상 변경
}

// 선택된 슬롯의 버튼 색상을 변경하는 메소드
void UPerkSlotWidget::UpdateSlotColor()
{
    if (!GameManager || !SlotButton)
    {
        return;
    }

    const TArray<FPerkData>& ChosenPerks = GameManager->GetChosenPerks();

    bool bIsSelected = false;
    for (const FPerkData& ExistingPerk : ChosenPerks)
    {
        if (ExistingPerk.PerkType == PerkData.PerkType)
        {
            bIsSelected = true;
            break;
        }
    }

    // 버튼 색상 변경
    FLinearColor NewColor = bIsSelected ? FLinearColor::Yellow : FLinearColor::White;
    SlotButton->SetBackgroundColor(NewColor);
}

// 마우스 호버 시 실행되는 메소드
void UPerkSlotWidget::OnHovered()
{
    if (PerkMenuWidgetRef)
    {
        FVector2D MousePos;
        GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y);

        PerkMenuWidgetRef->ShowTooltip(
            FText::FromString(PerkData.PerkName),
            FText::FromString(PerkData.PerkDescription),
            MousePos
        );
    }
}

// 마우스 언호버 시 실행되는 메소드
void UPerkSlotWidget::OnUnhovered()
{
    if (PerkMenuWidgetRef)
    {
        PerkMenuWidgetRef->HideTooltip();
    }
}