


#include "PerkEffector.h"
#include "PlayerCharacter.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"

// 선택된 퍽들을 적용하는 메소드
void APerkEffector::ApplyPerks(APlayerCharacter* PlayerCharacter, const TArray<FPerkData>& Perks)
{
    if (!PlayerCharacter || Perks.Num() == 0)
    {
        return;
    }

    for (const FPerkData& Perk : Perks)
    {
        switch (Perk.PerkType)
        {
        case EPerkType::ReinforcedBody:
            ApplyReinforcedBody(PlayerCharacter, Perk.PerkValue);
            break;

        case EPerkType::PlentifulAmmo:
            ApplyPlentifulAmmo(PlayerCharacter, Perk.PerkValue);
            break;

        case EPerkType::BonusLoot:
            ApplyBonusLoot(PlayerCharacter, Perk.PerkValue);
            break;

        case EPerkType::AutoShield:
            ApplyAutoShield(PlayerCharacter, Perk.PerkValue);
            break;

        default:
            break;
        }
    }
}

// 강화된 신체 퍽을 적용하는 메소드
void APerkEffector::ApplyReinforcedBody(APlayerCharacter* PlayerCharacter, float Value)
{
    if (!PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->SetMaxHP(PlayerCharacter->GetMaxHP() * Value, true);
}

// 넉넉한 탄약 퍽을 적용하는 메소드
void APerkEffector::ApplyPlentifulAmmo(APlayerCharacter* PlayerCharacter, float Value)
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
void APerkEffector::ApplyBonusLoot(APlayerCharacter* PlayerCharacter, float Value)
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
void APerkEffector::ApplyAutoShield(APlayerCharacter* PlayerCharacter, float Value)
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
    GetWorld()->GetTimerManager().SetTimer(ShieldTimerHandle, [this, PlayerCharacter, ShieldAmount]()
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