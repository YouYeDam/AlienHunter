

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnLoadGameClicked();

    UFUNCTION()
    void OnStartNewGameClicked();

	UFUNCTION()
	void OnConfirmNewGame();

	UFUNCTION()
	void OnPopupClose();

    UFUNCTION()
    void OnQuitGameClicked();

    void GameStart();
    
protected:
    virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PopupWidgetClass;

    UPROPERTY()
    UPopupWidget* PopupWidget;

    UPROPERTY(meta = (BindWidget))
    class UButton* LoadGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartNewGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitGameButton;
};
