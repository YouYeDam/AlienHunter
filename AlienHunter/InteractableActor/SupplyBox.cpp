


#include "SupplyBox.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Gun.h"
#include "Grenade.h"
#include "PerkEffector.h"

void ASupplyBox::Collect(AActor* Collector)
{
    Super::Collect(Collector);

    CalculatRandomValue();

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Collector);

    if (PlayerCharacter && HealKitCount > 0)
    {
        PlayerCharacter->IncreaseHealKitCount(HealKitCount);
    }

    AGun* PlayerGun = PlayerCharacter->GetEquippedGun();
    if (PlayerGun && AmmoValue > 0)
    {
        int32 AmmoCount = PlayerGun->GetMagazineSize() * AmmoValue;
        PlayerGun->IncreaseSpareAmmoCount(AmmoCount);
    }

    AGrenade* PlayerGrenade = PlayerCharacter->GetEquippedGrenade();
    if (PlayerGrenade && GrenadeCount > 0)
    {
        PlayerGrenade->IncreaseTotalGrenadeCount(GrenadeCount);
    }

    // 퍽 효과 실행
    if (PlayerCharacter->GetPerkEffector())
    {
        PlayerCharacter->GetPerkEffector()->TriggerFieldScavenger();
    }

    // 아이템 파괴
    Destroy();
}

void ASupplyBox::CalculatRandomValue()
{
    HealKitCount = FMath::RandRange(0, 3); // 0 ~ 3 중 랜덤 선택

    float AmmoValues[] = {0.0f, 0.5f, 0.7f, 1.0f};
    AmmoValue = AmmoValues[FMath::RandRange(0, 3)]; // 0, 0.5, 0.7, 1 중 랜덤 선택

    GrenadeCount = FMath::RandRange(0, 2); // 0 ~ 2 중 랜덤 선택

    // 둘 다 0이면 하나를 최소값으로 설정
    if (HealKitCount == 0 && AmmoValue == 0.0f)
    {
        if (FMath::RandBool())
        {
            HealKitCount = 1;
        }
        else
        {
            AmmoValue = 0.5f;
        }
    }
}
