

#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "GunItemData.generated.h"

// 총기류 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FGunItemData : public FBaseItemData
{
    GENERATED_BODY()

public:
	// 탄약 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoCount;

	// 여분 탄약 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SpareAmmoCount;

	// 사거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GunRange;

	// 반동 정도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GunRecoil;

	FGunItemData()
        : FBaseItemData()
        , AmmoCount(0)
        , SpareAmmoCount(0)
        , GunRange(0)
        , GunRecoil(0)
    {}
};
