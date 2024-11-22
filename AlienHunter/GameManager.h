// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemData.h"
#include "MissionData.h"
#include "GameManager.generated.h"

class UInventoryMenuWidget;

UCLASS(Blueprintable)
class ALIENHUNTER_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 Energy = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 EXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 EXPRequirementForLevelup = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Meta = (AllowPrivateAccess = "true"))
	int32 KillEnemyCount = 0;

	TArray<FItemData> PurchasedItems;

	UPROPERTY()
	UInventoryMenuWidget* InventoryMenuWidgetRef; // 인벤토리 메뉴 위젯 참조

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<class AActor> EquippedGunClass;

	FItemData EquippedGunItemData;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<class AActor> EquippedSwordClass;

	FItemData EquippedSwordItemData;
	FMissionData CurrentMissionData;

public:
	virtual void Init() override;

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

	void AddPurchasedItem(const FItemData& Item);

	const TArray<FItemData>& GetPurchasedItems() const;

	void SetInventoryMenuWidget(UInventoryMenuWidget* InventoryWidget);

	TSubclassOf<AActor> GetEquippedGun() const;
	void SetEquippedGun(TSubclassOf<AActor> NewGun);

	TSubclassOf<AActor> GetEquippedSword() const;
	void SetEquippedSword(TSubclassOf<AActor> NewSword);

	FItemData GetEquippedGunItemData() const;
	void SetEquippedGunItemData(const FItemData& NewGunItemData);

	FItemData GetEquippedSwordItemData() const;
	void SetEquippedSwordItemData(const FItemData& NewSwordItemData);

	FMissionData GetCurrentMissionData() const;
	void SetCurrentMissionData(const FMissionData& NewMissionData);

private:
	void Levelup();
};
