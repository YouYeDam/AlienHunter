

#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "GrenadeItemData.generated.h"

// 수류탄의 종류를 정의하는 Enum
UENUM()
enum class EGrenadeType : uint8
{
	None,			  // 기본값
	FireBomb,		  // 화염탄
};

// 수류탄류 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FGrenadeItemData : public FBaseItemData
{
	GENERATED_BODY()

public:
	// 수류탄 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalGrenadeCount;

	// 폭발 반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectRange;

	// 특수효과 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpecialEffectValue;

	// 특수효과 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpecialEffectDuration;

	// 수류탄 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGrenadeType GrenadeType;

	FGrenadeItemData()
        : FBaseItemData()
        , TotalGrenadeCount(0)
		, EffectRange(0)
		, SpecialEffectValue(0)
		, SpecialEffectDuration(0)
		, GrenadeType(EGrenadeType::None)
    {}
};
