


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MissionData.generated.h"

// 미션 데이터를 저장하는 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FMissionData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 미션 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    FString MissionName;

    // 미션 목표
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    FString MissionObject;

    // 미션 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    FString MissionDescription;

    // 불러올 미션 레벨
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    FString MissionLevel;

    // 미션 이미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    UTexture2D* MissionImage;

    // 미션 에너지 보상
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    int32 MissionEnergyReward;

    // 미션 경험치 보상
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    int32 MissionEXPReward;

    // 미션 아이디
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    int32 MissionID;

    FMissionData()
        : MissionName(TEXT(""))
        , MissionObject(TEXT(""))
        , MissionDescription(TEXT(""))
        , MissionImage(nullptr)
        , MissionEnergyReward(0)
        , MissionEXPReward(0)
        , MissionID(0)
    {}
};