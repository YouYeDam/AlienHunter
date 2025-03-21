


#include "GrenadeEffector.h"
#include "PlayerCharacter.h"
#include "Engine/DamageEvents.h"

// 수류탄 효과를 적용하는 메소드
void AGrenadeEffector::ApplayGrenadeEffect(APlayerCharacter* InPlayerCharacter, FGrenadeItemData* GrenadeData, const TArray<AActor*>& AffectedActors)
{
    if (!InPlayerCharacter || !GrenadeData || AffectedActors.Num() == 0)
    {
        return;
    }

    PlayerCharacter = InPlayerCharacter;

	switch (GrenadeData->GrenadeType)
	{
	case EGrenadeType::FireBomb:
		ApplyFireBombEffect(GrenadeData->SpecialEffectValue, GrenadeData->SpecialEffectDuration, AffectedActors);
		break;

	default:
		break;
	}
}

// 화염탄 효과를 적용하는 메소드
void AGrenadeEffector::ApplyFireBombEffect(float Value, float Duration, const TArray<AActor*>& AffectedActors)
{
    // 기존 타이머가 있으면 제거 (새로운 호출 발생 시 시간 갱신)
    GetWorld()->GetTimerManager().ClearTimer(FireBombTimerHandle);

    FireBombTick = 0;

	// 지속기간 동안 1초마다 효과 적용
    GetWorldTimerManager().SetTimer(FireBombTimerHandle, [this, AffectedActors, Value, Duration]()
    {
        for (AActor* Actor : AffectedActors)
        {
            FDamageEvent DamageEvent;
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            Actor->TakeDamage(Value, DamageEvent, PlayerController, this);
        }

		FireBombTick++;

        if (FireBombTick >= Duration)
        {
            GetWorldTimerManager().ClearTimer(FireBombTimerHandle);
        }

    }, 1.0f, true);
}

// 수류탄 이펙터가 제거될 경우 실행하는 메소드
void AGrenadeEffector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(FireBombTimerHandle); // 화염탄 타이머 삭제
    }
}


// 수류탄 이펙터가 GC에 의해 소멸될 경우 실행하는 메소드
void AGrenadeEffector::BeginDestroy()
{
    Super::BeginDestroy();

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(FireBombTimerHandle); // 화염탄 타이머 삭제
    }
}

