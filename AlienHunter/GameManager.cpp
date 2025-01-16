#include "GameManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "InventoryMenuWidget.h"
#include "Gun.h"
#include "Sword.h"

// 게임 시작 시 기본 보유한 아이템 초기화
void UGameManager::Init()
{
    Super::Init();

    if (!GunItemDataTable || !SwordItemDataTable)
    {
        return;
    }

    static const FString GunContextString(TEXT("GameManager Gun Item Initialization"));
	static const FString SwordContextString(TEXT("GameManager Sword Item Initialization"));

    // 플레이어 기본 총기류 장비 설정
    const int32 RifleGunID = 1; // RifleGun의 ID
    FGunItemData* GunItemData = GunItemDataTable->FindRow<FGunItemData>(FName(*FString::FromInt(RifleGunID)), GunContextString);
    if (GunItemData)
    {
        EquippedGunClass = GunItemData->ItemBlueprint;
        EquippedGunItemData = *GunItemData;
    }

    // 플레이어 기본 도검류 장비 설정
    const int32 KatanaID = 1; // Katana의 ID
    FSwordItemData* SwordItemData = SwordItemDataTable->FindRow<FSwordItemData>(FName(*FString::FromInt(KatanaID)), SwordContextString);
    if (SwordItemData)
    {
        EquippedSwordClass = SwordItemData->ItemBlueprint;
        EquippedSwordItemData = *SwordItemData;
    }

    // 기본 보유 장비를 구매 장비로 추가
    AddPurchasedGunItem(EquippedGunItemData);
    AddPurchasedSwordItem(EquippedSwordItemData);
}

void UGameManager::Shutdown()
{
    Super::Shutdown();
}

int32 UGameManager::GetInitialHealth() const
{
    return Health;
}

int32 UGameManager::GetEnergy() const
{
    return Energy;
}

void UGameManager::SetEnergy(int32 NewEnergy)
{
    Energy = NewEnergy;
}

int32 UGameManager::GetPlayerLevel() const
{
	return PlayerLevel;
}

void UGameManager::SetPlayerLevel(int32 NewPlayerLevel)
{
	PlayerLevel = NewPlayerLevel;
}

int32 UGameManager::GetEXP() const
{
	return EXP;
}

void UGameManager::SetEXP(int32 NewEXP)
{
	EXP = NewEXP;

	// 경험치가 레벨업 시 필요한 경험치보다 높으면 레벨업
	if (EXP >= EXPRequirementForLevelup) 
	{
		Levelup();
	}
}

int32 UGameManager::GetKillEnemyCount() const
{
	return KillEnemyCount;
}

void UGameManager::SetKillEnemyCount(int32 NewKillEnemyCount)
{
	KillEnemyCount = NewKillEnemyCount;
}

int32 UGameManager::GetEXPRequirementForLevelup() const
{
	return EXPRequirementForLevelup;
}

// 레벨업 시 처리하는 내용을 구성한 메소드
void UGameManager::Levelup()
{
	EXP -= EXPRequirementForLevelup;
	PlayerLevel++;
	EXPRequirementForLevelup *= 1.1; // 레벨업 시 필요한 경험치 1.1배 증가시키기
	Health += 50; // 레벨업마다 체력 50 증가

	// 2단 레벨업 방지 설정
	if (EXP >= EXPRequirementForLevelup)
	{
		EXP = EXPRequirementForLevelup - 1;
	}
}

// 임무 완료 시 보상을 처리하는 메소드
void UGameManager::GainMissionReward()
{
	Energy += CurrentMissionData.MissionEnergyReward;
	EXP += CurrentMissionData.MissionEXPReward;

	if (EXP >= EXPRequirementForLevelup)
	{
		Levelup();
	}
}

// 구매한 총기류 아이템을 추가하는 메소드
void UGameManager::AddPurchasedGunItem(const FGunItemData& Item)
{
    PurchasedGunItems.Add(Item);

	if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGunItemDataArray();
	}
}

// 구매한 도검류 아이템을 추가하는 메소드
void UGameManager::AddPurchasedSwordItem(const FSwordItemData& Item)
{
    PurchasedSwordItems.Add(Item);

	if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateSwordItemDataArray();
	}
}

// 인벤토리 메뉴 위젯의 참조를 설정하는 메소드
void UGameManager::SetInventoryMenuWidget(UInventoryMenuWidget* InventoryWidget)
{
    InventoryMenuWidgetRef = InventoryWidget;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGunDetails(EquippedGunItemData);
        InventoryMenuWidgetRef->UpdateSwordDetails(EquippedSwordItemData);
	}
}

const TArray<FGunItemData>& UGameManager::GetPurchasedGunItems() const
{
	return PurchasedGunItems;
}

const TArray<FSwordItemData>& UGameManager::GetPurchasedSwordItems() const
{
	return PurchasedSwordItems;
}

void UGameManager::SetEquippedGun(TSubclassOf<AActor> NewGun)
{
    EquippedGunClass = NewGun;
}

TSubclassOf<AActor> UGameManager::GetEquippedGun() const
{
    return EquippedGunClass;
}

void UGameManager::SetEquippedSword(TSubclassOf<AActor> NewSword)
{
    EquippedSwordClass = NewSword;
}

TSubclassOf<AActor> UGameManager::GetEquippedSword() const
{
    return EquippedSwordClass;
}

FGunItemData UGameManager::GetEquippedGunItemData() const
{
	return EquippedGunItemData;
}

void UGameManager::SetEquippedGunItemData(const FGunItemData& NewGunItemData)
{
	EquippedGunItemData = NewGunItemData;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGunDetails(EquippedGunItemData);
	}
}

FSwordItemData UGameManager::GetEquippedSwordItemData() const
{
	return EquippedSwordItemData;
}

void UGameManager::SetEquippedSwordItemData(const FSwordItemData& NewSwordItemData)
{
	EquippedSwordItemData = NewSwordItemData;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateSwordDetails(EquippedSwordItemData);
	}
}

FMissionData UGameManager::GetCurrentMissionData() const
{
	return CurrentMissionData;
}

void UGameManager::SetCurrentMissionData(const FMissionData& NewMissionData)
{
	CurrentMissionData = NewMissionData;
}

FName UGameManager::GetCurrentMissionName() const
{
	return CurrentMissionName;
}

void UGameManager::SetCurrentMissionName(const FName& NewMissionName)
{
	CurrentMissionName = NewMissionName;
}