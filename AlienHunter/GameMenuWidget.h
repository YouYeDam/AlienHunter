

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuGameMode.h"
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

protected:
    virtual void NativeConstruct() override;

private:
    AGameMenuGameMode* GameMode;

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
};
