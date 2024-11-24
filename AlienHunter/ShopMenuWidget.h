// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "PopupWidget.h"
#include "GameManager.h"
#include "ShopMenuWidget.generated.h"

UCLASS()
class ALIENHUNTER_API UShopMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnMoveToGameMenuClicked();

    UFUNCTION()
    void OnBuyItemClicked();

    UFUNCTION(BlueprintCallable)
    void UpdateItemDetails(const FItemData& ItemData);

	UFUNCTION()
	void OnConfirmPurchase();

	UFUNCTION()
	void OnPopupClose();

    void ShowCurrentEnergy();
    
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY()
    TArray<FItemData> ItemDataArray; // 아이템 데이터 배열

	UPROPERTY()
	UGameManager* GameManager;

    UPROPERTY()
    FItemData SelectedItem; // 선택된 아이템 데이터

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;
    
    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* BuyButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ShopScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemType;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemPrice;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDescription;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnergyAmount;

    // 슬롯 템플릿 클래스
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> ShopSlotClass;

    UPROPERTY()
    UPopupWidget* PopupWidget;

    void InitializeItemData();

    void CreateItemSlots();
};
