


#include "PopupWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

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
	}

	if (CancelButton)
	{
		if (HasCancelButton) // 취소 버튼이 필요한지 검사
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

