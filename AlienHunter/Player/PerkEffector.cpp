


#include "PerkEffector.h"
#include "PlayerCharacter.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// 선택된 퍽들을 적용하는 메소드
void APerkEffector::ApplyPerks(APlayerCharacter* InPlayerCharacter, const TArray<FPerkData>& Perks)
{
    if (!InPlayerCharacter || Perks.Num() == 0)
    {
        return;
    }

    PlayerCharacter = InPlayerCharacter;

    for (const FPerkData& Perk : Perks)
    {
        switch (Perk.PerkType)
        {
        case EPerkType::ReinforcedBody:
            ApplyReinforcedBody(Perk.PerkValue);
            break;

        case EPerkType::PlentifulAmmo:
            ApplyPlentifulAmmo(Perk.PerkValue);
            break;

        case EPerkType::BonusLoot:
            ApplyBonusLoot(Perk.PerkValue);
            break;

        case EPerkType::AutoShield:
            ApplyAutoShield(Perk.PerkValue);
            break;

        case EPerkType::TacticalRush:
            ApplyTacticalRush(Perk.PerkValue);
            break;

        case EPerkType::FieldScavenger:
            ApplyFieldScavenger(Perk.PerkValue);
            break;

        default:
            break;
        }
    }
}

// 강화된 신체 퍽을 적용하는 메소드
void APerkEffector::ApplyReinforcedBody(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->SetMaxHP(PlayerCharacter->GetMaxHP() * Value, true);
}

// 넉넉한 탄약 퍽을 적용하는 메소드
void APerkEffector::ApplyPlentifulAmmo(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    AGun* EquippedGun = PlayerCharacter->GetEquippedGun();
    if (!EquippedGun)
    {
        return;
    }
    
    EquippedGun->SetMagazineSize(EquippedGun->GetMagazineSize() * Value);

    int32 AmmoCount = EquippedGun->GetMagazineSize() * Value;
    EquippedGun->IncreaseSpareAmmoCount(AmmoCount);
}

// 추가 전리품 퍽을 적용하는 메소드
void APerkEffector::ApplyBonusLoot(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(PlayerCharacter->GetController());
    if (PlayerController)
    {
        PlayerController->SetBonusLootMultiplier(Value);
    }
}

// 자동 보호막 퍽을 적용하는 메소드
void APerkEffector::ApplyAutoShield(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }
    
    // 기존 타이머가 있으면 중복 실행 방지
    if (GetWorld()->GetTimerManager().IsTimerActive(ShieldTimerHandle))
    {
        return;
    }

    // 게임 시작시 바로 최초 쉴드 생성
    int32 ShieldAmount = PlayerCharacter->GetMaxHP() * 0.2f;
    if (PlayerCharacter->GetPlayerShield() < ShieldAmount)
    {
        
        PlayerCharacter->SetPlayerShield(ShieldAmount);
    }

    // 일정 시간마다 보호막을 갱신하는 타이머 실행
    GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, [this, ShieldAmount]()
    {
        if (!PlayerCharacter)
        {
            return;
        }
        
        if (PlayerCharacter->GetPlayerShield() < ShieldAmount)
        {
            
            PlayerCharacter->SetPlayerShield(ShieldAmount);
        }

    }, 10.0f, true); // 10초마다 반복 실행
}

// 전술적 돌입 퍽을 적용하는 메소드
void APerkEffector::ApplyTacticalRush(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    bCanApplyTacticalRush = true;
    TacticalRushValue = Value;
}

// 야전 수색자 퍽을 적용하는 메소드
void APerkEffector::ApplyFieldScavenger(float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    bCanApplyFieldScavenger = true;
    FieldScavengerValue = Value;
}

// 전술적 돌입 퍽의 기능을 실제 실행하는 메소드
void APerkEffector::TriggerTacticalRush()
{
    if (!bCanApplyTacticalRush || !PlayerCharacter)
    {
        return;
    }

    // 기존 타이머가 있으면 제거 (새로운 처치 발생 시 시간 갱신)
    GetWorld()->GetTimerManager().ClearTimer(TacticalRushTimerHandle);

    // 원래 이동 속도를 저장 (초기 실행 시만 저장)
    if (OriginalWalkSpeed == 0.0f)
    {
        OriginalWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
    }

    // 이동 속도 증가
    float NewSpeed = OriginalWalkSpeed * TacticalRushValue;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

    // 5초 후 원래 속도로 복귀하는 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(TacticalRushTimerHandle, this, &APerkEffector::ResetTacticalRush, 3.0f, false);
}

// 이동 속도를 원래대로 복구하는 메소드
void APerkEffector::ResetTacticalRush()
{
    if (!PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

// 야전 수색자 퍽의 기능을 실제 실행하는 메소드
void APerkEffector::TriggerFieldScavenger()
{
    if (!bCanApplyFieldScavenger || !PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->SetGainedEnergy(PlayerCharacter->GetGainedEnergy() + FieldScavengerValue);
}