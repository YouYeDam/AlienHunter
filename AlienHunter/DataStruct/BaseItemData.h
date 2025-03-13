

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseItemData.generated.h"

// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FBaseItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
     // 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

     // 아이템 세부설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;

     // 아이템 타입(총기류, 도검류)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemType;

     // 아이템 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemPrice;

     // 아이템 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemDamage;

     // 아이템 이미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemImage;

     // 아이템 액터(블루프린트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AActor> ItemBlueprint;

    FBaseItemData()
        : ItemName(TEXT(""))
        , ItemDescription(TEXT(""))
        , ItemType(NAME_None)
        , ItemPrice(0)
        , ItemDamage(0)
        , ItemImage(nullptr)
        , ItemBlueprint(nullptr)
    {}
};
