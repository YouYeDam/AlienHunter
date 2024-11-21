// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "GameManager.h"
#include "InventoryMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UInventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void OnMoveToGameMenuClicked();

    UFUNCTION()
    void OnEquipItemClicked();

    UFUNCTION(BlueprintCallable)
    void UpdateItemDetails(const FItemData& ItemData);

    UFUNCTION(BlueprintCallable)
    void UpdateGunDetails(const FItemData& ItemData);

    UFUNCTION(BlueprintCallable)
    void UpdateSwordDetails(const FItemData& ItemData);

    UFUNCTION(BlueprintCallable)
    void UpdateItemDataArray();

protected:
    virtual void NativeConstruct() override;

private:
    TArray<FItemData> ItemDataArray;
    
	UPROPERTY()
	UGameManager* GameManager;

    FItemData SelectedItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;
	
    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* EquipItemButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* InventoryScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* GunImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* SwordImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GunName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SwordName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GunDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SwordDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDescription;

    // 슬롯 템플릿 클래스
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> InventorySlotClass;

	void CreateInventorySlots();
};
