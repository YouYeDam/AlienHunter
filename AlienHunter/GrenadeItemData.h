

#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "GrenadeItemData.generated.h"

// 수류탄류 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FGrenadeItemData : public FBaseItemData
{
	GENERATED_BODY()

public:
	// 수류탄 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrenadeCount;

	// 폭발 반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectRange;

	// 특수효과 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpecialEffectValue;

	FGrenadeItemData()
        : FBaseItemData()
        , GrenadeCount(0)
		, EffectRange(0)
		, SpecialEffectValue(0)
    {}
};
