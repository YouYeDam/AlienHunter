


#include "TooltipWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UTooltipWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UTooltipWidget::SetTooltipData(const FText& NewTitle, const FText& NewDescription, UTexture2D* NewImage)
{
    if (TooltipTitleText)
    {
        TooltipTitleText->SetText(NewTitle);
    }

    if (TooltipDescriptionText)
    {
        TooltipDescriptionText->SetText(NewDescription);
    }

    if (TooltipImage && NewImage)
    {
        TooltipImage->SetBrushFromTexture(NewImage);
    }
}


