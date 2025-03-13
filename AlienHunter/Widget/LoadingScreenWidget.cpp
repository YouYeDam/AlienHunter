


#include "LoadingScreenWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MissionData.h"
#include "Kismet/GameplayStatics.h"

void ULoadingScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// 로딩 진행 상태를 업데이트하는 메소드
void ULoadingScreenWidget::UpdateLoadingText(bool bIsComplete)
{
    if (!LoadingText)
    {
        return;
    }

    if (bIsComplete)
    {
        LoadingText->SetText(FText::FromString(TEXT("로딩 완료! 아무 키나 눌러 미션을 시작하세요."))); // 로딩 완료 메시지
    }
    else
    {
        LoadingText->SetText(FText::FromString(TEXT("로딩 중"))); // 로딩 중 메시지
    }
}

// 로딩화면 배경 이미지를 설정하는 메소드
void ULoadingScreenWidget::UpdateBackground(UTexture2D* InBackgroundImage)
{
    if (BackgroundImage && InBackgroundImage)
    {
        BackgroundImage->SetBrushFromTexture(InBackgroundImage);
    }
}

// 미션 이름을 표시하는 메소드
void ULoadingScreenWidget::ShowMissionName()
{
    if (MissionNameText && GameManager)
    {
        FMissionData PrevMissionData = GameManager->GetCurrentMissionData();
        MissionNameText->SetText(FText::FromString(PrevMissionData.MissionName));
    }
}
