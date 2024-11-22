// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "ShopMenuWidget.h"
#include "ShopSlotWidget.generated.h"
/**
 * 
 */
UCLASS()
class ALIENHUNTER_API UShopSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void OnSlotClicked();

    void InitializeSlot(UShopMenuWidget* InShopMenuWidget, const FItemData& InItemData);

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UShopMenuWidget* ShopMenuWidgetRef;

    UPROPERTY()
    FItemData ItemData;
};
