

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GunItemData.h"
#include "SwordItemData.h"
#include "MissionData.h"
#include "PerkData.h"
#include "GameManager.generated.h"

class UInventoryMenuWidget;
class AGun;
class ASword;

UCLASS(Blueprintable)
class ALIENHUNTER_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	int32 Health = 100;

	UPROPERTY(EditAnywhere)
	int32 Energy = 0;

	UPROPERTY(EditAnywhere)
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere)
	int32 EXP = 0;

	UPROPERTY(EditAnywhere)
	int32 EXPRequirementForLevelup = 100;

	UPROPERTY(EditAnywhere)
	int32 KillEnemyCount = 0;

	UPROPERTY(EditAnywhere)
	int32 CompleteMissionCount = 0;

    UPROPERTY(EditAnywhere)
    UDataTable* GunItemDataTable;

    UPROPERTY(EditAnywhere)
    UDataTable* SwordItemDataTable;

	UPROPERTY()
	TArray<FGunItemData> PurchasedGunItems;

	UPROPERTY()
	TArray<FSwordItemData> PurchasedSwordItems;

	UPROPERTY()
	TArray<FPerkData> ChosenPerks;

	UPROPERTY()
	UInventoryMenuWidget* InventoryMenuWidgetRef; // 인벤토리 메뉴 위젯 참조

	UPROPERTY()
	TSubclassOf<class AActor> EquippedGunClass;

	UPROPERTY()
	FGunItemData EquippedGunItemData;

	UPROPERTY()
	TSubclassOf<class AActor> EquippedSwordClass;

	UPROPERTY()
	FSwordItemData EquippedSwordItemData;
	
	UPROPERTY()
	FMissionData CurrentMissionData;

	UPROPERTY()
	FName CurrentMissionLevel;

	UPROPERTY()
	bool PrevMissionSuccess;

	UPROPERTY()
	int32 PrevMissionEnergy;

	UPROPERTY()
	int32 PrevMissionEXP;

	UPROPERTY()
	int32 PrevCombatEnergy;

	UPROPERTY()
	int32 PrevCombatEXP;

	UPROPERTY()
	int32 PrevEnemyKillCount;

	// 사운드 설정
	UPROPERTY()
    float MasterVolume = 1.0f;

	UPROPERTY()
    float SFXVolume = 1.0f;

	UPROPERTY()
    float BGMVolume = 1.0f;

	UPROPERTY()
    float UIVolume = 1.0f;
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	bool SaveGame();

	UFUNCTION(BlueprintCallable)
	bool LoadGame();

	UFUNCTION(BlueprintCallable)
	void DeleteGame();

	UFUNCTION(BlueprintCallable)
	int32 GetInitialHealth() const;

	UFUNCTION(BlueprintCallable)
	int32 GetEnergy() const;

	UFUNCTION(BlueprintCallable)
	void SetEnergy(int32 NewEnergy);

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintCallable)
	void SetPlayerLevel(int32 NewPlayerLevel);

	UFUNCTION(BlueprintCallable)
	int32 GetEXP() const;

	UFUNCTION(BlueprintCallable)
	void SetEXP(int32 NewEXP);

	UFUNCTION(BlueprintCallable)
	int32 GetKillEnemyCount() const;

	UFUNCTION(BlueprintCallable)
	void SetKillEnemyCount(int32 NewKillEnemyCount);

	UFUNCTION(BlueprintCallable)
	int32 GetCompleteMissionCount() const;

	UFUNCTION(BlueprintCallable)
	void SetCompleteMissionCount(int32 NewCompleteMissionCount);
	
	UFUNCTION(BlueprintCallable)
	int32 GetEXPRequirementForLevelup() const;

	void GainMissionReward();

	void AddPurchasedGunItem(const FGunItemData& Item);
	const TArray<FGunItemData>& GetPurchasedGunItems() const;

	void AddPurchasedSwordItem(const FSwordItemData& Item);
	const TArray<FSwordItemData>& GetPurchasedSwordItems() const;

	void AddChosenPerks(const FPerkData& Perk);
	const TArray<FPerkData>& GetChosenPerks() const;
	void ClearShosenPerks();

	void SetInventoryMenuWidget(UInventoryMenuWidget* InventoryWidget);

	TSubclassOf<AActor> GetEquippedGun() const;
	void SetEquippedGun(TSubclassOf<AActor> NewGun);

	TSubclassOf<AActor> GetEquippedSword() const;
	void SetEquippedSword(TSubclassOf<AActor> NewSword);

	FGunItemData GetEquippedGunItemData() const;
	void SetEquippedGunItemData(const FGunItemData& NewGunItemData);

	FSwordItemData GetEquippedSwordItemData() const;
	void SetEquippedSwordItemData(const FSwordItemData& NewSwordItemData);

	FMissionData GetCurrentMissionData() const;
	void SetCurrentMissionData(const FMissionData& NewMissionData);

	FName GetCurrentMissionLevel() const;
	void SetCurrentMissionLevel(const FName& NewMissionLevel);

	UFUNCTION()
	bool GetPrevMissionSuccess() const;

	UFUNCTION()
	void SetPrevMissionSuccess(bool NewMissionSuccess);

	UFUNCTION()
	int32 GetPrevMissionEnergy() const;

	UFUNCTION()
	void SetPrevMissionEnergy(int32 NewEnergy);

	UFUNCTION()
	int32 GetPrevMissionEXP() const;

	UFUNCTION()
	void SetPrevMissionEXP(int32 NewEXP);

	UFUNCTION()
	int32 GetPrevCombatEnergy() const;

	UFUNCTION()
	void SetPrevCombatEnergy(int32 NewEnergy);

	UFUNCTION()
	int32 GetPrevCombatEXP() const;

	UFUNCTION()
	void SetPrevCombatEXP(int32 NewEXP);

	UFUNCTION()
	int32 GetPrevEnemyKillCount() const;

	UFUNCTION()
	void SetPrevEnemyKillCount(int32 NewEnemyKillCount);

	// 사운드 설정
    float GetMasterVolume() const;
    void SetMasterVolume(float Value);

    float GetSFXVolume() const;
    void SetSFXVolume(float Value);

    float GetBGMVolume() const;
    void SetBGMVolume(float Value);

    float GetUIVolume() const;
    void SetUIVolume(float Value);

private:
	void Levelup();
};
