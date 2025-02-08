


#include "TooltipWidget.h"
#include "Components/TextBlock.h"

void UTooltipWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UTooltipWidget::SetTooltipData(const FText& NewTitle, const FText& NewDescription)
{
    if (TooltipTitleText)
    {
        TooltipTitleText->SetText(NewTitle);
    }

    if (TooltipDescriptionText)
    {
        TooltipDescriptionText->SetText(NewDescription);
    }
}

