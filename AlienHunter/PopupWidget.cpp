// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

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

void UPopupWidget::OnConfirmClicked()
{
	ConfirmClicked.Broadcast();
	RemoveFromParent();
}

void UPopupWidget::OnCancelClicked()
{
	CancelClicked.Broadcast();
	RemoveFromParent();
}

