

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"


UCLASS()
class ALIENHUNTER_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetTooltipData(const FText& NewTitle, const FText& NewDescription);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TooltipTitleText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TooltipDescriptionText;
};
