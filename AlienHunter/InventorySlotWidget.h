

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseItemData.h"
#include "GunItemData.h"
#include "SwordItemData.h"
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

    void InitializeGunSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FGunItemData& InGunItemData);
    void InitializeSwordSlot(UInventoryMenuWidget* InInventoryMenuWidget, const FSwordItemData& InSwordItemData);

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemNameText;
    
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;
    
    UInventoryMenuWidget* InventoryMenuWidgetRef; // 인벤토리 메뉴 위젯 참조

    UPROPERTY()
    FBaseItemData ItemData;

    FGunItemData GunItemData;

    FSwordItemData SwordItemData;
};
