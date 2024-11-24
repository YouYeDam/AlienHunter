

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "InventoryMenuWidget.h"
#include "InventorySlotWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void OnSlotClicked();

    void InitializeSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FItemData& InItemData);

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UInventoryMenuWidget* InventoryMenuWidgetRef; // 인벤토리 메뉴 위젯 참조

    FItemData ItemData;
};
