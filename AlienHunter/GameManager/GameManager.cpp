


#include "GameManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameSaveData.h"
#include "InventoryMenuWidget.h"
#include "Weapon/Gun.h"
#include "Sword.h"
#include "Grenade.h"
#include "MainPlayerController.h"

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
    static const FString GrenadeContextString(TEXT("GameManager Grenade Item Initialization"));

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

    // 플레이어 기본 수류탄류 장비 설정
    const int32 BombID = 1; // Bomb의 ID
    FGrenadeItemData* GrenadeItemData = GrenadeItemDataTable->FindRow<FGrenadeItemData>(FName(*FString::FromInt(BombID)), GrenadeContextString);
    if (GrenadeItemData)
    {
        EquippedGrenadeClass = GrenadeItemData->ItemBlueprint;
        EquippedGrenadeItemData = *GrenadeItemData;
    }

    // 기본 보유 장비를 구매 장비로 추가
    AddPurchasedGunItem(EquippedGunItemData);
    AddPurchasedSwordItem(EquippedSwordItemData);
    AddPurchasedGrenadeItem(EquippedGrenadeItemData);
}

void UGameManager::Shutdown()
{
    Super::Shutdown();
}

// 게임을 저장하는 메소드
bool UGameManager::SaveGame()
{
    UGameSaveData* SaveGameData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));

    if (!SaveGameData)
    {
        return false;
    }

    // 플레이어 상태 저장
    SaveGameData->SetHealth(Health);
    SaveGameData->SetEnergy(Energy);
    SaveGameData->SetPlayerLevel(PlayerLevel);
    SaveGameData->SetEXP(EXP);
    SaveGameData->SetEXPRequirementForLevelup(EXPRequirementForLevelup);
    SaveGameData->SetKillEnemyCount(KillEnemyCount);

    // 인벤토리 상태 저장
    SaveGameData->SetPurchasedGunItems(PurchasedGunItems);
    SaveGameData->SetPurchasedSwordItems(PurchasedSwordItems);
    SaveGameData->SetPurchasedGrenadeItems(PurchasedGrenadeItems);

    SaveGameData->SetEquippedGunClass(EquippedGunClass);
    SaveGameData->SetEquippedGunItemData(EquippedGunItemData);

    SaveGameData->SetEquippedSwordClass(EquippedSwordClass);
    SaveGameData->SetEquippedSwordItemData(EquippedSwordItemData);

    SaveGameData->SetEquippedGrenadeClass(EquippedGrenadeClass);
    SaveGameData->SetEquippedGrenadeItemData(EquippedGrenadeItemData);

    // 퍽 상태 저장
    SaveGameData->SetChosenPerks(ChosenPerks);

    // 사운드 설정 저장
    SaveGameData->SetMasterVolume(MasterVolume);
    SaveGameData->SetSFXVolume(SFXVolume);
    SaveGameData->SetBGMVolume(BGMVolume);
    SaveGameData->SetUIVolume(UIVolume);

    // 게임 저장
	#if WITH_EDITOR
		FString SlotName = TEXT("EditorSaveSlot");
	#else
		FString SlotName = TEXT("SaveSlot");
	#endif

    bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveGameData, SlotName, 0);

    return bSaveSuccess; // 저장 성공 여부 반환
}

// 저장한 게임을 불러오는 메소드
bool UGameManager::LoadGame()
{
    // 슬롯 이름 설정
    #if WITH_EDITOR
        FString SlotName = TEXT("EditorSaveSlot");
    #else
        FString SlotName = TEXT("SaveSlot");
    #endif

    // 저장된 데이터를 로드
    UGameSaveData* LoadedGame = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

    if (!LoadedGame)
    {
        return false;
    }

    // 플레이어 상태 복원
    Health = LoadedGame->GetHealth();
    Energy = LoadedGame->GetEnergy();
    PlayerLevel = LoadedGame->GetPlayerLevel();
    EXP = LoadedGame->GetEXP();
    EXPRequirementForLevelup = LoadedGame->GetEXPRequirementForLevelup();
    KillEnemyCount = LoadedGame->GetKillEnemyCount();

    // 인벤토리 상태 복원
    PurchasedGunItems = LoadedGame->GetPurchasedGunItems();
    PurchasedSwordItems = LoadedGame->GetPurchasedSwordItems();
    PurchasedGrenadeItems = LoadedGame->GetPurchasedGrenadeItems();

    EquippedGunClass = LoadedGame->GetEquippedGunClass();
    EquippedGunItemData = LoadedGame->GetEquippedGunItemData();

    EquippedSwordClass = LoadedGame->GetEquippedSwordClass();
    EquippedSwordItemData = LoadedGame->GetEquippedSwordItemData();

    EquippedGrenadeClass = LoadedGame->GetEquippedGrenadeClass();
    EquippedGrenadeItemData = LoadedGame->GetEquippedGrenadeItemData();

    // 퍽 상태 복원
    ChosenPerks = LoadedGame->GetChosenPerks();

    // 사운드 설정 상태 복원
    MasterVolume = LoadedGame->GetMasterVolume();
    SFXVolume = LoadedGame->GetSFXVolume();
    BGMVolume = LoadedGame->GetBGMVolume();
    UIVolume = LoadedGame->GetUIVolume();

    return true;
}

// 저장한 게임을 삭제하는 메소드
void UGameManager::DeleteGame()
{
    // 슬롯 이름 설정
    #if WITH_EDITOR
        FString SlotName = TEXT("EditorSaveSlot");
    #else
        FString SlotName = TEXT("SaveSlot");
    #endif

    UGameplayStatics::DeleteGameInSlot(SlotName, 0);
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

int32 UGameManager::GetCompleteMissionCount() const
{
    return CompleteMissionCount;
}

void UGameManager::SetCompleteMissionCount(int32 NewCompleteMissionCount)
{
    CompleteMissionCount = NewCompleteMissionCount;
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
	EXPRequirementForLevelup *= 1.25; // 레벨업 시 필요한 경험치 1.1배 증가시키기
	Health += 10; // 레벨업마다 체력 10 증가

	// 2단 레벨업 방지 설정
	if (EXP >= EXPRequirementForLevelup)
	{
		EXP = EXPRequirementForLevelup - 1;
	}
}

// 임무 완료 시 보상을 처리하는 메소드
void UGameManager::GainMissionReward()
{
    // 현재 플레이어 컨트롤러 가져오기
    AMainPlayerController* PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    float BonusMultiplier = (PlayerController) ? PlayerController->GetBonusLootMultiplier() : 1.0f;

    // 보너스 배율 적용
    Energy += CurrentMissionData.MissionEnergyReward * BonusMultiplier;
    EXP += CurrentMissionData.MissionEXPReward * BonusMultiplier;

    // 경험치가 레벨업 요구량을 초과하면 레벨업
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

const TArray<FGunItemData>& UGameManager::GetPurchasedGunItems() const
{
	return PurchasedGunItems;
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

const TArray<FSwordItemData>& UGameManager::GetPurchasedSwordItems() const
{
	return PurchasedSwordItems;
}

// 구매한 수류탄류 아이템을 추가하는 메소드
void UGameManager::AddPurchasedGrenadeItem(const FGrenadeItemData& Item)
{
    PurchasedGrenadeItems.Add(Item);

	if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGrenadeItemDataArray();
	}
}

const TArray<FGrenadeItemData>& UGameManager::GetPurchasedGrenadeItems() const
{
    return PurchasedGrenadeItems;
}

void UGameManager::AddChosenPerks(const FPerkData& Perk)
{
    ChosenPerks.Add(Perk);
}

const TArray<FPerkData>& UGameManager::GetChosenPerks() const
{
    return ChosenPerks;
}

void UGameManager::ClearShosenPerks()
{
    ChosenPerks.Empty();
}

// 인벤토리 메뉴 위젯의 참조를 설정하는 메소드
void UGameManager::SetInventoryMenuWidget(UInventoryMenuWidget* InventoryWidget)
{
    InventoryMenuWidgetRef = InventoryWidget;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGunDetails(EquippedGunItemData);
        InventoryMenuWidgetRef->UpdateSwordDetails(EquippedSwordItemData);
        InventoryMenuWidgetRef->UpdateGrenadeDetails(EquippedGrenadeItemData);
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

void UGameManager::SetEquippedGrenade(TSubclassOf<AActor> NewGrenade)
{
    EquippedGrenadeClass = NewGrenade;
}

TSubclassOf<AActor> UGameManager::GetEquippedGrenade() const
{
    return EquippedGrenadeClass;
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

FGrenadeItemData UGameManager::GetEquippedGrenadeItemData() const
{
    return EquippedGrenadeItemData;
}

void UGameManager::SetEquippedGrenadeItemData(const FGrenadeItemData& NewGrenadeItemData)
{
	EquippedGrenadeItemData = NewGrenadeItemData;

    if (InventoryMenuWidgetRef)
	{
		InventoryMenuWidgetRef->UpdateGrenadeDetails(EquippedGrenadeItemData);
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

FName UGameManager::GetCurrentMissionLevel() const
{
	return CurrentMissionLevel;
}

void UGameManager::SetCurrentMissionLevel(const FName& NewMissionLevel)
{
	CurrentMissionLevel = NewMissionLevel;
}

bool UGameManager::GetPrevMissionSuccess() const
{
    return PrevMissionSuccess;
}

void UGameManager::SetPrevMissionSuccess(bool NewPrevMissionSuccess)
{
    PrevMissionSuccess = NewPrevMissionSuccess;
}

int32 UGameManager::GetPrevMissionEnergy() const
{
    return PrevMissionEnergy;
}

void UGameManager::SetPrevMissionEnergy(int32 NewEnergy)
{
    PrevMissionEnergy = NewEnergy;
}

int32 UGameManager::GetPrevMissionEXP() const
{
    return PrevMissionEXP;
}

void UGameManager::SetPrevMissionEXP(int32 NewEXP)
{
    PrevMissionEXP = NewEXP;
}

int32 UGameManager::GetPrevCombatEnergy() const
{
    return PrevCombatEnergy;
}

void UGameManager::SetPrevCombatEnergy(int32 NewEnergy)
{
    PrevCombatEnergy = NewEnergy;
}

int32 UGameManager::GetPrevCombatEXP() const
{
    return PrevCombatEXP;
}

void UGameManager::SetPrevCombatEXP(int32 NewEXP)
{
    PrevCombatEXP = NewEXP;
}

int32 UGameManager::GetPrevEnemyKillCount() const
{
    return PrevEnemyKillCount;
}

void UGameManager::SetPrevEnemyKillCount(int32 NewEnemyKillCount)
{
    PrevEnemyKillCount = NewEnemyKillCount;
}

float UGameManager::GetMasterVolume() const
{
    return MasterVolume;
}

void UGameManager::SetMasterVolume(float Value)
{
    MasterVolume = Value;
}

float UGameManager::GetSFXVolume() const
{
    return SFXVolume;
}

void UGameManager::SetSFXVolume(float Value)
{
    SFXVolume = Value;
}

float UGameManager::GetBGMVolume() const
{
    return BGMVolume;
}

void UGameManager::SetBGMVolume(float Value)
{
    BGMVolume = Value;
}

float UGameManager::GetUIVolume() const
{
    return UIVolume;
}

void UGameManager::SetUIVolume(float Value)
{
    UIVolume = Value;
}