


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionData.h"
#include "GameManager.h"
#include "MissionMenuWidget.generated.h"

UCLASS()
class ALIENHUNTER_API UMissionMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION()
    void OnMoveToGameMenuClicked();

    UFUNCTION()
    void OnStartMissionClicked();

    UFUNCTION(BlueprintCallable)
    void UpdateMissionDetails(const FMissionData& MissionData);

protected:
    virtual void NativeConstruct() override;

private:
    TArray<FMissionData> MissionDataArray; // 미션 데이터 배열

    FMissionData SelectedMissionData; // 선택된 미션 데이터
    
	UPROPERTY()
	UGameManager* GameManager;

    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartMissionButton;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* MissionScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* MissionImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionObject;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionReward;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionDescription;

    FName SelectedMissionLevel;

    // 슬롯 템플릿 클래스
    UPROPERTY(EditAnywhere, Category = "Mission")
    TSubclassOf<class UUserWidget> MissionSlotClass;

    void InitializeMissionData();

    void CreateMissionSlots();
};
