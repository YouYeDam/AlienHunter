

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "HelpInfoData.h"
#include "HelpMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UHelpMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnMoveToReturnToMenuMenuClicked();

	UFUNCTION()
	void OnMoveTabClicked();

	UFUNCTION()
	void OnCombatTabClicked();

	UFUNCTION()
	void OnInteractTabClicked();

	void CreateHelpSlots(const TArray<FHelpInfoData>& HelpDataArray);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MoveTabButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CombatTabButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* InteractTabButton;

	UPROPERTY(meta = (BindWidget))
    class UScrollBox* HelpScrollBox;

	UPROPERTY(EditAnywhere)
    UDataTable* HelpInfoDataTable;// 도움말 정보 데이터 테이블

    UPROPERTY()
    TArray<FHelpInfoData> MoveHelpInfoDataArray; // 이동 도움말 정보 데이터 배열

	UPROPERTY()
    TArray<FHelpInfoData> CombatHelpInfoDataArray; // 전투 도움말 정보 데이터 배열

    UPROPERTY()
    TArray<FHelpInfoData> InteractHelpInfoDataArray; // 상호작용 도움말 정보 데이터 배열

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> HelpSlotClass; // 슬롯 템플릿 클래스

	void InitializeInfoData();
};
