

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkMenuWidget.h"
#include "PerkData.h"
#include "GameManager.h"
#include "PerkSlotWidget.generated.h"

UCLASS()
class ALIENHUNTER_API UPerkSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void OnSlotClicked();

	void InitializeSlot(UPerkMenuWidget* InPerkMenuWidget, const FPerkData& InPerkData);

    void UpdateSlotColor();
    
private:
    UPROPERTY()
    UGameManager* GameManager;

    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PerkNameText;

    UPerkMenuWidget* PerkMenuWidgetRef;  // 퍽 메뉴 위젯 참조

    UPROPERTY()
    FPerkData PerkData;

    UFUNCTION()
    void OnHovered();

    UFUNCTION()
    void OnUnhovered();
};
