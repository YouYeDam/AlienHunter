

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeItemData.h"
#include "GrenadeEffector.generated.h"

class APlayerCharacter;

UCLASS()
class ALIENHUNTER_API AGrenadeEffector : public AActor
{
	GENERATED_BODY()

public:
	void ApplayGrenadeEffect(APlayerCharacter* InPlayerCharacter, FGrenadeItemData* GrenadeData, const TArray<AActor*>& AffectedActors);

private:
	void ApplyFireBombEffect(float Value, float Duration, const TArray<AActor*>& AffectedActors);

private:
	FTimerHandle FireBombTimerHandle; // 화염탄 효과 지속을 위한 타이머
	int32 FireBombTick = 0;

	APlayerCharacter* PlayerCharacter;
};
