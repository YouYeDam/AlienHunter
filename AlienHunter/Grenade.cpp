


#include "Grenade.h"
#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GrenadeItemData.h"

AGrenade::AGrenade()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
		
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

    // 플레이어 무기일 경우 데이터 초기화
    if (bIsPlayerWeapon && GrenadeDataTable)
    {
        InitializeWeaponData(); // 데이터 초기화 함수 호출
    }
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
	
    if (bIsPlayerWeapon && GrenadeDataTable)
    {
        InitializeWeaponData(); // 데이터 초기화 함수 호출
    }
}

// 조건에 따라 메시의 가시성을 재설정하는 메소드
void AGrenade::SetMeshVisibility(bool bVisible)
{
	if (Mesh)
    {
        Mesh->SetVisibility(bVisible);
    }
}

// 무기 데이터 값을 받아 속성값을 초기화하는 메소드
void AGrenade::InitializeWeaponData()
{
    if (!GrenadeDataTable || WeaponID <= 0)
    {
        return;
    }

    // DataTable에서 무기 데이터를 검색
    static const FString ContextString(TEXT("Weapon Data Initialization"));
    FGrenadeItemData* GrenadeData = GrenadeDataTable->FindRow<FGrenadeItemData>(FName(*FString::FromInt(WeaponID)), ContextString);

    if (GrenadeData)
    {
        // 무기 속성 초기화
        Damage = GrenadeData->ItemDamage;
		GrenadeCount = GrenadeData->GrenadeCount;
		EffectRange = GrenadeData->EffectRange;
		SpecialEffectValue = GrenadeData->SpecialEffectValue;
    }
}

int32 AGrenade::GetGrenadeCount() const
{
	return GrenadeCount;
}
