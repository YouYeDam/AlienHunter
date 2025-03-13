

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseItemData.h"
#include "GunItemData.h"
#include "SwordItemData.h"
#include "GrenadeItemData.h"
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

    UFUNCTION()
    void OnGunTabClicked();

    UFUNCTION()
    void OnSwordTabClicked();

    UFUNCTION()
    void OnGrenadeTabClicked();

    UFUNCTION()
    void UpdateItemDetails(const FBaseItemData& ItemData);

    UFUNCTION()
    void UpdateGunDetails(const FGunItemData& ItemData);

    UFUNCTION()
    void UpdateSwordDetails(const FSwordItemData& ItemData);

    UFUNCTION()
    void UpdateGrenadeDetails(const FGrenadeItemData& ItemData);

    UFUNCTION()
    void UpdateGunItemDataArray();

    UFUNCTION()
    void UpdateSwordItemDataArray();

    UFUNCTION()
    void UpdateGrenadeItemDataArray();

    void ResetSelectedItems();

    void SetSelectedGunItem(const FGunItemData& GunItem);

    void SetSelectedSwordItem(const FSwordItemData& SwordItem);

    void SetSelectedGrenadeItem(const FGrenadeItemData& GrenadeItem);

    void CreateGunInventorySlots();

    void CreateSwordInventorySlots();

    void CreateGrenadeInventorySlots();
    
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY()
    TArray<FGunItemData> GunItemDataArray; // 총기류 아이템 데이터 배열

    UPROPERTY()
    TArray<FSwordItemData> SwordItemDataArray; // 도검류 아이템 데이터 배열

    UPROPERTY()
    TArray<FGrenadeItemData> GrenadeItemDataArray; // 수류탄류 아이템 데이터 배열

	UPROPERTY()
	UGameManager* GameManager;

    UPROPERTY()
    FBaseItemData SelectedItem; // 선택된 아이템 데이터

    UPROPERTY()
    FGunItemData SelectedGunItem; // 선택된 총기류 아이템 데이터

    UPROPERTY()
    FSwordItemData SelectedSwordItem; // 선택된 도검류 아이템 데이터

    UPROPERTY()
    FGrenadeItemData SelectedGrenadeItem; // 선택된 수류류 아이템 데이터

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;
	
    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* EquipItemButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GunTabButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SwordTabButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GrenadeTabButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* InventoryScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* GunImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* SwordImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* GrenadeImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GunName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SwordName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GrenadeName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GunDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SwordDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GrenadeDamage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ItemDescription;

    // 슬롯 템플릿 클래스
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> InventorySlotClass;
};
