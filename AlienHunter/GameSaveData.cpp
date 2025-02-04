


#include "GameSaveData.h"

// 플레이어 상태
int32 UGameSaveData::GetHealth() const 
{
    return Health;
}

void UGameSaveData::SetHealth(int32 NewHealth) 
{
    Health = NewHealth;
}

int32 UGameSaveData::GetEnergy() const 
{
    return Energy;
}

void UGameSaveData::SetEnergy(int32 NewEnergy) 
{
    Energy = NewEnergy;
}

int32 UGameSaveData::GetPlayerLevel() const 
{
    return PlayerLevel;
}

void UGameSaveData::SetPlayerLevel(int32 NewPlayerLevel) 
{
    PlayerLevel = NewPlayerLevel;
}

int32 UGameSaveData::GetEXP() const 
{
    return EXP;
}

void UGameSaveData::SetEXP(int32 NewEXP) 
{
    EXP = NewEXP;
}

int32 UGameSaveData::GetEXPRequirementForLevelup() const 
{
    return EXPRequirementForLevelup;
}

void UGameSaveData::SetEXPRequirementForLevelup(int32 NewRequirement) 
{
    EXPRequirementForLevelup = NewRequirement;
}

int32 UGameSaveData::GetKillEnemyCount() const 
{
    return KillEnemyCount;
}

void UGameSaveData::SetKillEnemyCount(int32 NewKillCount) 
{
    KillEnemyCount = NewKillCount;
}

// 인벤토리 상태
TArray<FGunItemData> UGameSaveData::GetPurchasedGunItems() const 
{
    return PurchasedGunItems;
}

void UGameSaveData::SetPurchasedGunItems(const TArray<FGunItemData>& NewGunItems) 
{
    PurchasedGunItems = NewGunItems;
}

TArray<FSwordItemData> UGameSaveData::GetPurchasedSwordItems() const 
{
    return PurchasedSwordItems;
}

void UGameSaveData::SetPurchasedSwordItems(const TArray<FSwordItemData>& NewSwordItems) 
{
    PurchasedSwordItems = NewSwordItems;
}

TSubclassOf<class AActor> UGameSaveData::GetEquippedGunClass() const 
{
    return EquippedGunClass;
}

void UGameSaveData::SetEquippedGunClass(TSubclassOf<class AActor> NewGunClass) 
{
    EquippedGunClass = NewGunClass;
}

FGunItemData UGameSaveData::GetEquippedGunItemData() const 
{
    return EquippedGunItemData;
}

void UGameSaveData::SetEquippedGunItemData(const FGunItemData& NewGunData) 
{
    EquippedGunItemData = NewGunData;
}

TSubclassOf<class AActor> UGameSaveData::GetEquippedSwordClass() const 
{
    return EquippedSwordClass;
}

void UGameSaveData::SetEquippedSwordClass(TSubclassOf<class AActor> NewSwordClass) 
{
    EquippedSwordClass = NewSwordClass;
}

FSwordItemData UGameSaveData::GetEquippedSwordItemData() const 
{
    return EquippedSwordItemData;
}

void UGameSaveData::SetEquippedSwordItemData(const FSwordItemData& NewSwordData) 
{
    EquippedSwordItemData = NewSwordData;
}

TArray<FPerkData> UGameSaveData::GetChosenPerks() const
{
    return ChosenPerks;
}

void UGameSaveData::SetChosenPerks(const TArray<FPerkData>& NewPerks)
{
    ChosenPerks = NewPerks;
}
