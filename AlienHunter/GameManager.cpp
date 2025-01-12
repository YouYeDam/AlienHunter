// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "InventoryMenuWidget.h"
#include "Engine/World.h"

// 게임 시작 시 기본 보유한 아이템 초기화
void UGameManager::Init()
{
    Super::Init();

    if (!ItemDataTable)
    {
        return;
    }

    static const FString ContextString(TEXT("GameManager Item Initialization"));

    // 플레이어 기본 총기류 장비 설정
    FItemData* GunItemData = ItemDataTable->FindRow<FItemData>(FName("RifleGun"), ContextString);
    if (GunItemData)
    {
        EquippedGunClass = GunItemData->ItemBlueprint;
        EquippedGunItemData = *GunItemData;
    }

    // 플레이어 기본 도검류 장비 설정
    FItemData* SwordItemData = ItemDataTable->FindRow<FItemData>(FName("Katana"), ContextString);
    if (SwordItemData)
    {
        EquippedSwordClass = SwordItemData->ItemBlueprint;
        EquippedSwordItemData = *SwordItemData;
    }

    // 기본 보유 장비를 구매 장비로 추가
    AddPurchasedItem(EquippedGunItemData);
    AddPurchasedItem(EquippedSwordItemData);
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

// 구매한 아이템을 추가하는 메소드
void UGameManager::AddPurchasedItem(const FItemData& Item)
{
    PurchasedItems.Add(Item);

	if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateItemDataArray();
	}
}

const TArray<FItemData>& UGameManager::GetPurchasedItems() const
{
    return PurchasedItems;
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

FItemData UGameManager::GetEquippedGunItemData() const
{
	return EquippedGunItemData;
}

void UGameManager::SetEquippedGunItemData(const FItemData& NewGunItemData)
{
	EquippedGunItemData = NewGunItemData;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGunDetails(EquippedGunItemData);
	}
}

FItemData UGameManager::GetEquippedSwordItemData() const
{
	return EquippedSwordItemData;
}

void UGameManager::SetEquippedSwordItemData(const FItemData& NewSwordItemData)
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
