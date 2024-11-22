

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void OnStartGameClicked();

    UFUNCTION()
    void OnQuitGameClicked();

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitGameButton;
};
