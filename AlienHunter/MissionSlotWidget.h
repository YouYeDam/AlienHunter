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

    UMissionMenuWidget* MissionMenuWidgetRef;

    UPROPERTY()
    FMissionData MissionData;
};
