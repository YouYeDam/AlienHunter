#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GunItemData.h"
#include "SwordItemData.h"
#include "GrenadeItemData.h"
#include "PerkData.h"
#include "GameSaveData.generated.h"

UCLASS()
class ALIENHUNTER_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()

private:
	// 플레이어 상태
	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 Health;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 Energy;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 PlayerLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 EXP;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 EXPRequirementForLevelup;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 KillEnemyCount;

	// 인벤토리 상태
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FGunItemData> PurchasedGunItems;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FSwordItemData> PurchasedSwordItems;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FGrenadeItemData> PurchasedGrenadeItems;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TSubclassOf<class AActor> EquippedGunClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	FGunItemData EquippedGunItemData;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TSubclassOf<class AActor> EquippedSwordClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	FSwordItemData EquippedSwordItemData;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TSubclassOf<class AActor> EquippedGrenadeClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	FGrenadeItemData EquippedGrenadeItemData;

	// 퍽 상태
	UPROPERTY(VisibleAnywhere, Category = "Perk")
	TArray<FPerkData> ChosenPerks;

	// 사운드 설정
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float MasterVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float SFXVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float BGMVolume;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	float UIVolume;

public:
	// 플레이어 상태
	int32 GetHealth() const;
	void SetHealth(int32 NewHealth);

	int32 GetEnergy() const;
	void SetEnergy(int32 NewEnergy);

	int32 GetPlayerLevel() const;
	void SetPlayerLevel(int32 NewPlayerLevel);

	int32 GetEXP() const;
	void SetEXP(int32 NewEXP);

	int32 GetEXPRequirementForLevelup() const;
	void SetEXPRequirementForLevelup(int32 NewRequirement);

	int32 GetKillEnemyCount() const;
	void SetKillEnemyCount(int32 NewKillCount);

	// 인벤토리 상태
	TArray<FGunItemData> GetPurchasedGunItems() const;
	void SetPurchasedGunItems(const TArray<FGunItemData>& NewGunItems);

	TArray<FSwordItemData> GetPurchasedSwordItems() const;
	void SetPurchasedSwordItems(const TArray<FSwordItemData>& NewSwordItems);

	TArray<FGrenadeItemData> GetPurchasedGrenadeItems() const;
	void SetPurchasedGrenadeItems(const TArray<FGrenadeItemData>& NewGrenadeItems);

	TSubclassOf<class AActor> GetEquippedGunClass() const;
	void SetEquippedGunClass(TSubclassOf<class AActor> NewGunClass);

	FGunItemData GetEquippedGunItemData() const;
	void SetEquippedGunItemData(const FGunItemData& NewGunData);

	TSubclassOf<class AActor> GetEquippedSwordClass() const;
	void SetEquippedSwordClass(TSubclassOf<class AActor> NewSwordClass);

	FSwordItemData GetEquippedSwordItemData() const;
	void SetEquippedSwordItemData(const FSwordItemData& NewSwordData);

	TSubclassOf<class AActor> GetEquippedGrenadeClass() const;
	void SetEquippedGrenadeClass(TSubclassOf<class AActor> NewGrenadeClass);

	FGrenadeItemData GetEquippedGrenadeItemData() const;
	void SetEquippedGrenadeItemData(const FGrenadeItemData& NewGrenadeData);

	// 퍽 상태
	TArray<FPerkData> GetChosenPerks() const;
	void SetChosenPerks(const TArray<FPerkData>& NewPerks);

	// 사운드 설정
    float GetMasterVolume() const;
    void SetMasterVolume(float Value);

    float GetSFXVolume() const;
    void SetSFXVolume(float Value);

    float GetBGMVolume() const;
    void SetBGMVolume(float Value);

    float GetUIVolume() const;
    void SetUIVolume(float Value);
};
