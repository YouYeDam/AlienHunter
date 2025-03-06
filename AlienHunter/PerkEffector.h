#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerkData.h"
#include "PerkEffector.generated.h"

class APlayerCharacter;

UCLASS()
class ALIENHUNTER_API APerkEffector : public AActor
{
    GENERATED_BODY()

public:
    void ApplyPerks(APlayerCharacter* PlayerCharacter, const TArray<FPerkData>& Perks);

private:
    FTimerHandle ShieldTimerHandle; // 보호막 생성을 위한 타이머

    void ApplyReinforcedBody(APlayerCharacter* PlayerCharacter, float Value);

    void ApplyPlentifulAmmo(APlayerCharacter* PlayerCharacter, float Value);

    void ApplyBonusLoot(APlayerCharacter* PlayerCharacter, float Value);

    void ApplyAutoShield(APlayerCharacter* PlayerCharacter, float Value);

    void ApplyTacticalRush(APlayerCharacter* PlayerCharacter, float Value);

    void ApplyFieldScavenger(APlayerCharacter* PlayerCharacter, float Value);
};
