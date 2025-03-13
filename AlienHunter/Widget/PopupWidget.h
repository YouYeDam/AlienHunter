

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPopupClicked); // 팝업 클릭을 위한 델리게이트 선언

UCLASS()
class ALIENHUNTER_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PopupText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget, Optional))
	class UButton* CancelButton;

protected:
	virtual void NativeConstruct() override;
	
public:
	void InitializePopup(FText Message, bool HasCancelButton);

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

	UPROPERTY(BlueprintAssignable)
	FPopupClicked ConfirmClicked;

	UPROPERTY(BlueprintAssignable)
	FPopupClicked CancelClicked;
};
