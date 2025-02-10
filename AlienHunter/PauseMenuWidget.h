

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.h"
#include "PauseMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HelpMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SoundSettingButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* MissionGiveupButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ReturnToGameButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;
    
    UPROPERTY()
    UPopupWidget* PopupWidget;

public:
	UFUNCTION()
	void OnMoveToHelpMenuClicked();

	UFUNCTION()
	void OnSountSettingClicked();

	UFUNCTION()
	void OnMissionGiveupClicked();

	UFUNCTION()
	void OnReturnToGameClicked();

	UFUNCTION()
	void OnConfirmMissionGiveup();

	UFUNCTION()
	void OnPopupClose();
};
