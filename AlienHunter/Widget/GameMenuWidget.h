

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuGameMode.h"
#include "PopupWidget.h"
#include "GameMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnMoveToMissionMenuClicked();

    UFUNCTION()
    void OnMoveToShopMenuClicked();

    UFUNCTION()
    void OnMoveToInventoryMenuClicked();

    UFUNCTION()
    void OnMoveToCharacterMenuClicked();

    UFUNCTION()
    void OnSaveGameClicked();

    UFUNCTION()
    void OnMoveToSoundMenuClicked();

    UFUNCTION()
    void OnQuitGameClicked();
    
	UFUNCTION()
	void OnConfirmQuitGame();

	UFUNCTION()
	void OnPopupClose();

protected:
    virtual void NativeConstruct() override;

private:
    AGameMenuGameMode* GameMode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;

    UPROPERTY()
    UPopupWidget* PopupWidget;

    UPROPERTY(meta = (BindWidget))
    class UButton* MissionMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ShopMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InventoryMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CharacterMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SaveGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SoundMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitGameButton;

private:
    bool bIsGameQuit = false;
};
