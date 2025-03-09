

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

// 몬스터 데이터 구조체
USTRUCT(BlueprintType)
struct ALIENHUNTER_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 몬스터 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MonsterName;
 
	// 몬스터 타입(근접류, 원거리류)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterType;

	// 몬스터 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MonsterMaxHP;

	// 몬스터 에너지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterEnergy;

	// 몬스터 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterEXP;

	// 몬스터 무기 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterWeaponDamage;

	// 몬스터 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterSpeed;

	// 몬스터 액터(블루프린트)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> MonsterBlueprint;
 
	FMonsterData()
		: MonsterName(TEXT(""))
		, MonsterType(NAME_None)
		, MonsterMaxHP(0)
		, MonsterEnergy(0)
		, MonsterEXP(0)
		, MonsterWeaponDamage(0)
		, MonsterSpeed(0)
		, MonsterBlueprint(nullptr)
	{}
};
