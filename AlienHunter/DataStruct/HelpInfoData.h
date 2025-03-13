

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HelpInfoData.generated.h"

// 도움말 정보 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FHelpInfoData : public FTableRowBase
{
	GENERATED_BODY()

public:
     // 도움말 정보 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HelpInfoName;

     // 도움말 정보 세부설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HelpInfoNameDescription;

	// 도움말 정보 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HelpInfoType;

    FHelpInfoData()
        : HelpInfoName(TEXT(""))
        , HelpInfoNameDescription(TEXT(""))
		, HelpInfoType(TEXT(""))
    {}
};
