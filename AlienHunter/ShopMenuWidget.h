

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "BaseItemData.h"
#include "GunItemData.h"
#include "SwordItemData.h"
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

    UFUNCTION()
    void OnGunTabClicked();

    UFUNCTION()
    void OnSwordTabClicked();

    UFUNCTION(BlueprintCallable)
    void UpdateItemDetails(const FBaseItemData& ItemData);

	UFUNCTION()
	void OnConfirmPurchase();

	UFUNCTION()
	void OnConfirmEquip();

	UFUNCTION()
	void OnPopupClose();

    void ShowCurrentEnergy();
    
    void ResetSelectedItems();

    void SetSelectedGunItem(const FGunItemData& GunItem);

    void SetSelectedSwordItem(const FSwordItemData& SwordItem);

    void CreateGunInventorySlots();

    void CreateSwordInventorySlots();
    
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(EditAnywhere)
    UDataTable* GunItemDataTable; // 총기류 아이템 데이터 테이블

    UPROPERTY(EditAnywhere)
    UDataTable* SwordItemDataTable;// 도검류 아이템 데이터 테이블

    UPROPERTY()
    TArray<FGunItemData> GunItemDataArray; // 총기류 아이템 데이터 배열

    UPROPERTY()
    TArray<FSwordItemData> SwordItemDataArray; // 도검류 아이템 데이터 배열

	UPROPERTY()
	UGameManager* GameManager;

    UPROPERTY()
    FBaseItemData SelectedItem; // 선택된 아이템 데이터

    UPROPERTY()
    FGunItemData SelectedGunItem; // 선택된 총기류 아이템 데이터

    UPROPERTY()
    FSwordItemData SelectedSwordItem; // 선택된 도검류 아이템 데이터

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;
    
    UPROPERTY()
    UPopupWidget* PopupWidget;
    
    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* BuyButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GunTabButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SwordTabButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ShopScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemTypeText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemPriceText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDamageText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnergyAmountText;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> ShopSlotClass; // 슬롯 템플릿 클래스

    void InitializeItemData();

    bool CheckItemAlreadyPurchased() const;
};
