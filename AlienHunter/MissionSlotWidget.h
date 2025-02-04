// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionData.h"
#include "MissionMenuWidget.h"
#include "MissionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHUNTER_API UMissionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION()
    void OnSlotClicked();

    void InitializeSlot(UMissionMenuWidget* InMissionMenuWidget, const FMissionData& InMissionData);

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionNameText;

    UMissionMenuWidget* MissionMenuWidgetRef; // 미션 메뉴 위젯 참조

    UPROPERTY()
    FMissionData MissionData; // 슬롯의 미션 데이터
};
