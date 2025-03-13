

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "MissionSummaryWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UMissionSummaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void OnConfirmClicked();

	UFUNCTION()
	void ShowResult();

protected:
    virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UGameManager* GameManager;
	
    UPROPERTY(meta = (BindWidget))
    class UImage* MissionImage;

    UPROPERTY(meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionResultText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionRewardText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GainedEnergyText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GainedEXPText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KillEnemyCountText;
};
