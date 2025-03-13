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

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void BeginDestroy() override;

private:
    FTimerHandle ShieldTimerHandle; // 보호막 생성을 위한 타이머

    FTimerHandle TacticalRushTimerHandle; // 전술적 돌입을 위한 타이머

    void ApplyReinforcedBody(float Value);

    void ApplyPlentifulAmmo(float Value);

    void ApplyBonusLoot(float Value);

    void ApplyAutoShield(float Value);

    void ApplyTacticalRush(float Value);

    void ApplyFieldScavenger(float Value);

public:
    void TriggerTacticalRush();

    void TriggerFieldScavenger();
    
private:
    APlayerCharacter* PlayerCharacter;

    // 전술적 돌입 퍽을 위한 변수
    bool bCanApplyTacticalRush = false;
    float TacticalRushValue = 0.0f;
    float OriginalWalkSpeed = 0.0f;

    // 야전 수색자 퍽을 위한 변수
    bool bCanApplyFieldScavenger = false;
    float FieldScavengerValue = 0.0f;


    void ResetTacticalRush();
};
