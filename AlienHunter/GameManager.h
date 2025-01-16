

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GunItemData.h"
#include "SwordItemData.h"
#include "MissionData.h"
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
    UDataTable* GunItemDataTable;

    UPROPERTY(EditAnywhere)
    UDataTable* SwordItemDataTable;

	UPROPERTY()
	TArray<FGunItemData> PurchasedGunItems;

	UPROPERTY()
	TArray<FSwordItemData> PurchasedSwordItems;

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
	FName CurrentMissionName;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

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
	int32 GetEXPRequirementForLevelup() const;

	void GainMissionReward();

	void AddPurchasedGunItem(const FGunItemData& Item);

	void AddPurchasedSwordItem(const FSwordItemData& Item);

	const TArray<FGunItemData>& GetPurchasedGunItems() const;

	const TArray<FSwordItemData>& GetPurchasedSwordItems() const;

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

	FName GetCurrentMissionName() const;
	void SetCurrentMissionName(const FName& NewMissionName);

private:
	void Levelup();
};
