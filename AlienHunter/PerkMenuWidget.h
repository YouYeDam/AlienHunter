

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkData.h"
#include "GameManager.h"
#include "PopupWidget.h"
#include "PerkMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UPerkMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnMoveToCharacterMenuButtonClicked();

    UFUNCTION()
    void OnPerkResetButtonClicked();

	UFUNCTION()
	void OnConfirmResetPerks();

	UFUNCTION()
	void OnPopupClose();

    void UpdatePerkSlots();
    
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(EditAnywhere)
    UDataTable* PerkDataTable; // 퍽 데이터 테이블

    UPROPERTY()
    TArray<FPerkData> PerkDataArray; // 퍽 데이터 배열

    UPROPERTY()
    UGameManager* GameManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;

    UPROPERTY()
    UPopupWidget* PopupWidget;

    UPROPERTY(meta = (BindWidget))
    class UButton* CharacterMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* PerkResetButton;

    // 5개의 가로 박스 (각 레벨별 퍽 슬롯을 담을 컨테이너)
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* PerkBoxLV1;
    
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* PerkBoxLV5;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* PerkBoxLV10;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* PerkBoxLV15;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* PerkBoxLV20;

    // 슬롯 템플릿 클래스
    UPROPERTY(EditAnywhere, Category = "Perk")
    TSubclassOf<class UUserWidget> PerkSlotClass;

    void InitializePerkData();

    void CreatePerkSlots();
};
