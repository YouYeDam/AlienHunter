// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseItemData.h"
#include "GunItemData.h"
#include "SwordItemData.h"
#include "ShopMenuWidget.h"
#include "ShopSlotWidget.generated.h"

UCLASS()
class ALIENHUNTER_API UShopSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void OnSlotClicked();

    void InitializeSlot(UShopMenuWidget* InShopMenuWidget, const FBaseItemData& InItemData);
    void InitializeGunSlot(UShopMenuWidget* InShopMenuWidget, const FGunItemData& InGunItemData);
    void InitializeSwordSlot(UShopMenuWidget* InShopMenuWidget, const FSwordItemData& InSwordItemData);

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UShopMenuWidget* ShopMenuWidgetRef;  // 상점 메뉴 위젯 참조

    UPROPERTY()
    FBaseItemData ItemData;
    FGunItemData GunItemData;
    FSwordItemData SwordItemData;
};
