

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "LoadingScreenWidget.generated.h"

UCLASS()
class ALIENHUNTER_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateLoadingText(bool bIsComplete);

    UFUNCTION(BlueprintCallable)
    void UpdateBackground(UTexture2D* InBackgroundImage);

    UFUNCTION()
    void ShowMissionName();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LoadingText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionNameText;

private:
	UPROPERTY()
	UGameManager* GameManager;
};
