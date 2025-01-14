

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
	// 탄창 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MagazineSize;

	// 탄창 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MagazineCount;

	// 사거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GunRange;

	// 반동 정도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GunRecoil;

	FGunItemData()
        : FBaseItemData()
        , MagazineSize(0)
        , MagazineCount(0)
        , GunRange(0)
        , GunRecoil(0)
    {}
};
