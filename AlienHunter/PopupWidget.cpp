


#include "PopupWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

void UPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

// 팝업창을 초기화하는 메소드
void UPopupWidget::InitializePopup(FText Message, bool HasCancelButton)
{
    if (PopupText)
    {
        PopupText->SetText(Message);
    }

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UPopupWidget::OnConfirmClicked);

        if (!HasCancelButton && ConfirmButton->Slot)
        {
            UCanvasPanelSlot* ConfirmSlot = Cast<UCanvasPanelSlot>(ConfirmButton->Slot);
            if (ConfirmSlot)
            {
                FVector2D CurrentPosition = ConfirmSlot->GetPosition();
                ConfirmSlot->SetPosition(FVector2D(CurrentPosition.X + 175.0f, CurrentPosition.Y)); // 확인 버튼만 존재할 경우 X축으로 버튼을 이동
            }
        }
    }

    if (CancelButton)
    {
        if (HasCancelButton)
        {
            CancelButton->SetVisibility(ESlateVisibility::Visible);
            CancelButton->OnClicked.AddDynamic(this, &UPopupWidget::OnCancelClicked);
        }
        else
        {
            CancelButton->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}


// 확인 버튼 클릭 시 동작하는 메소드
void UPopupWidget::OnConfirmClicked()
{
	ConfirmClicked.Broadcast(); // 확인 버튼 클릭 델리게이트 실행
	RemoveFromParent();
}

// 취소 버튼 클릭 시 동작하는 메소드
void UPopupWidget::OnCancelClicked()
{
	CancelClicked.Broadcast(); // 취소 버튼 클릭 델리게이트 실행
	RemoveFromParent();
}

