

#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "SwordItemData.generated.h"

// 도검류 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FSwordItemData : public FBaseItemData
{
    GENERATED_BODY()

public:
    FSwordItemData()
        : FBaseItemData()
    {}
};