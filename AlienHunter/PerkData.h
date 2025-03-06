

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PerkData.generated.h"

// 퍽의 종류를 정의하는 Enum
UENUM()
enum class EPerkType : uint8
{
	None,			  // 기본값
    ReinforcedBody,   // 강화된 신체
    PlentifulAmmo,    // 넉넉한 탄약
    BonusLoot,        // 추가 전리품
    AutoShield,       // 자동 보호막
    TacticalRush,     // 전술적 돌입
    FieldScavenger,   // 야전 수색자
};

// 퍽 데이터를 저장하는 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FPerkData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 퍽 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perk")
    FString PerkName;

    // 퍽 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perk")
    FString PerkDescription;

    // 퍽 효과 값
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perk")
    float PerkValue;

	// 퍽 요구 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perk")
	int32 PerkRequiredLevel;

	// 퍽 종류
    UPROPERTY(EditAnywhere, Category = "Perk")
    EPerkType PerkType;

    // 퍽 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perk")
	UTexture2D* PerkImage;

    FPerkData()
        : PerkName(TEXT(""))
        , PerkDescription(TEXT(""))
        , PerkValue(0.0f)
		, PerkRequiredLevel(0)
		, PerkType(EPerkType::None)
        , PerkImage(nullptr)
    {}
};
