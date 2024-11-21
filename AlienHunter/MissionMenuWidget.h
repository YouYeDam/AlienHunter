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

    // 미션 슬롯에 따른 세부 사항을 업데이트 하는 함수
    UFUNCTION(BlueprintCallable)
    void UpdateMissionDetails(const FMissionData& MissionData);

protected:
    virtual void NativeConstruct() override;

private:
    // 미션 데이터 배열
    TArray<FMissionData> MissionDataArray;

    FMissionData SelectedMissionData;
    
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

    // 미션 데이터를 초기화하는 함수
    void InitializeMissionData();

    // 슬롯을 동적으로 생성하고 UI에 추가하는 함수
    void CreateMissionSlots();
};
