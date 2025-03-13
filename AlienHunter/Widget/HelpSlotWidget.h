

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HelpMenuWidget.h"
#include "HelpInfoData.h"
#include "HelpSlotWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UHelpSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
	void InitializeSlot(UHelpMenuWidget* InHelpMenuWidgetRef, const FHelpInfoData& InHelpInfoData);

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HelpInfoText;

	UHelpMenuWidget* HelpMenuWidgetRef;  // 도움말 메뉴 위젯 참조

	FHelpInfoData HelpInfoData;
};
