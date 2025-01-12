
#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class ALIENHUNTER_API AMonsterCharacter : public AMainCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int32 EXP = 0; // 몬스터가 제공하는 경험치

	UPROPERTY(EditDefaultsOnly)
	int32 Energy = 0; // 몬스터가 제공하는 에너지
private:

public:
	int32 GetEnergy() const;
	int32 GetEXP() const;
};
