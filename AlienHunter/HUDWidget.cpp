


#include "HUDWidget.h"
#include "PlayerCharacter.h"
#include "Gun.h"
#include "Grenade.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CollectObjectGameMode.h"
#include "EnemyKillGameMode.h"

void UHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateGainedEnergyText();
    UpdateGainedEXPText();

    UpdateHealKitText();
    UpdateAmmoText();
    UpdateGrenadeCountText();
}

// 획득한 에너지 텍스트를 갱신하는 메소드
void UHUDWidget::UpdateGainedEnergyText()
{
    if (PlayerCharacter && GainedEnergy)
    {
        GainedEnergy->SetText(FText::Format(
            NSLOCTEXT("HUD", "EnergyText", "획득한 에너지: {0}"),
            FText::AsNumber(PlayerCharacter->GetGainedEnergy())
        ));
    }
}

// 획득한 경험치 텍스트를 갱신하는 메소드
void UHUDWidget::UpdateGainedEXPText()
{
    if (PlayerCharacter && GainedEXP)
    {
        GainedEXP->SetText(FText::Format(
            NSLOCTEXT("HUD", "EXPText", "획득한 경험치: {0}"),
            FText::AsNumber(PlayerCharacter->GetGainedEXP())
        ));
    }
}

// 플레이어의 회복 키트 현황을 갱신하드 메소드
void UHUDWidget::UpdateHealKitText()
{
    if (PlayerCharacter && HealKit)
    {
        HealKit->SetText(FText::Format(
            NSLOCTEXT("HUD", "HealKitText", "회복 키트: {0}"),
            FText::AsNumber(PlayerCharacter->GetHealKitCount())
        ));
    }
}

// 플레이어의 탄약 현황을 갱신하는 메소드
void UHUDWidget::UpdateAmmoText()
{
    if (PlayerCharacter && Ammo)
    {
        AGun* EquippedGun = PlayerCharacter->GetEquippedGun();
        if (EquippedGun)
        {
            Ammo->SetText(FText::Format(
                NSLOCTEXT("HUD", "AmmoText", "탄약: {0} / {1}"),
                FText::AsNumber(EquippedGun->GetAmmoCount()),
                FText::AsNumber(EquippedGun->GetSpareAmmoCount())
            ));
        }
    }
}

void UHUDWidget::UpdateGrenadeCountText()
{
    if (PlayerCharacter && GrenadeCount)
    {
        AGrenade* EquippedGrenade = PlayerCharacter->GetEquippedGrenade();
        if (EquippedGrenade)
        {
            GrenadeCount->SetText(FText::Format(
                NSLOCTEXT("HUD", "GrenadeText", "수류탄: {0} / {1}"),
                FText::AsNumber(EquippedGrenade->GetGrenadeCount()),
                FText::AsNumber(EquippedGrenade->GetTotalGrenadeCount())
            ));
        }
    }
}

// 특정 게임 모드에 따라 미션 진행 상태를 갱신하는 메소드
void UHUDWidget::UpdateMissionProgressText()
{
    if (MissionProgress)
    {
        // 현재 게임 모드를 가져오기
        AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
        if (!CurrentGameMode)
        {
            return;
        }

        if (AEnemyKillGameMode* KillGameMode = Cast<AEnemyKillGameMode>(CurrentGameMode))
        {
            int32 DeadEnemies = KillGameMode->GetDeadEnemyCount();
            int32 TargetEnemies = KillGameMode->GetTargetEnemyCount();

            MissionProgress->SetText(FText::Format(
                NSLOCTEXT("HUD", "KillProgressText", "처치 진행: {0} / {1}"),
                FText::AsNumber(DeadEnemies),
                FText::AsNumber(TargetEnemies)
            ));
        }

        else if (ACollectObjectGameMode* CollectGameMode = Cast<ACollectObjectGameMode>(CurrentGameMode))
        {
            int32 CollectedCount = CollectGameMode->GetCurrentCollectingCount();
            int32 RequiredCount = CollectGameMode->GetRequiredCollectingCount();

            MissionProgress->SetText(FText::Format(
                NSLOCTEXT("HUD", "MissionProgressText", "수집 진행: {0} / {1}"),
                FText::AsNumber(CollectedCount),
                FText::AsNumber(RequiredCount)
            ));
        }
    }
}