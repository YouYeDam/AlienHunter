


#include "Grenade.h"
#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "PlayerCharacter.h"
#include "GrenadeEffector.h"

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

    PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AGrenade::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
	
    if (bIsPlayerWeapon && GrenadeDataTable)
    {
        InitializeWeaponData(); // 데이터 초기화 함수 호출
    }
}

AController* AGrenade::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) {
		return nullptr;
	}
	return OwnerPawn->GetController();
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
    GrenadeData = GrenadeDataTable->FindRow<FGrenadeItemData>(FName(*FString::FromInt(WeaponID)), ContextString);

    if (GrenadeData)
    {
        // 무기 속성 초기화
        Damage = GrenadeData->ItemDamage;
		TotalGrenadeCount = GrenadeData->TotalGrenadeCount;
		EffectRange = GrenadeData->EffectRange;
		SpecialEffectValue = GrenadeData->SpecialEffectValue;
        SpecialEffectDuration = GrenadeData->SpecialEffectDuration;

        MaxGrenadeCount = TotalGrenadeCount;

        GrenadeCount = 1;
        TotalGrenadeCount--;
    }
}

// 수류탄을 재장전하는 메소드
bool AGrenade::ReloadGrenade()
{
    if (TotalGrenadeCount <= 0 || GrenadeCount == 1) // 여분 수류탄이 없거나 이미 장전 상태이면 장전 불가
    {
        return false;
    }

    TotalGrenadeCount--;
    GrenadeCount = 1;

    if (ReloadSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
    }

    SetMeshVisibility(true);

    return true;
}

// 3.5초 후에 Explode() 실행하는 타이머를 설정하는 메소드
void AGrenade::ReadyExplosion()
{
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AGrenade::Explode, 3.5f, false);
}

// 수류탄을 폭발시키는 메소드
void AGrenade::Explode()
{
    FVector ExplosionLocation = Mesh->GetComponentLocation();

    // "Enemy" 태그를 가진 액터들 찾기
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), OverlappingActors);

    // 범위 내 액터들 저장
    TArray<AActor*> FilteredActors;

    for (AActor* HitActor : OverlappingActors)
    {
        if (!HitActor)
        {
            continue;
        }

        float Distance = FVector::Dist(ExplosionLocation, HitActor->GetActorLocation());

        // 반경 내에 있는 경우만 처리
        if (Distance > EffectRange)
        {
            continue;
        }
        else
        {
            FilteredActors.Add(HitActor);
        }

        // 거리 기반 피해량 계산 (최소 50% 피해)
        float DamageMultiplier = 1.0f - (Distance / EffectRange) * 0.5f;
        int32 AppliedDamage = FMath::CeilToInt(Damage * DamageMultiplier);

        AController* OwnerController = GetOwnerController();
        FDamageEvent DamageEvent;
        HitActor->TakeDamage(AppliedDamage, DamageEvent, OwnerController, this);
    }

    // 수류탄의 고유 효과 실행
    if (PlayerCharacter && PlayerCharacter->GetGrenadeEffector())
    {
        PlayerCharacter->GetGrenadeEffector()->ApplayGrenadeEffect(PlayerCharacter, GrenadeData, FilteredActors);
    }

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, ExplosionLocation);
    }

    if (ExtraEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExtraEffect, ExplosionLocation);
    }

    // 폭발 사운드 재생
    if (ExplosionSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, ExplosionLocation);
    }

    // 수류탄 제거
    Destroy();
}

// 수류탄 차지 시 효과음을 재생하는 메소드
void AGrenade::PlayChargeSound()
{
    if (GrenadeChargeSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GrenadeChargeSound, GetActorLocation());
    }
}

UStaticMeshComponent* AGrenade::GetMesh() const
{
    return Mesh;
}

int32 AGrenade::GetGrenadeCount() const
{
	return GrenadeCount;
}

void AGrenade::SetGrenadeCount(int32 Count)
{
    GrenadeCount = Count;
}

int32 AGrenade::GetTotalGrenadeCount() const
{
    return TotalGrenadeCount;
}

void AGrenade::IncreaseTotalGrenadeCount(int32 Count)
{
    if (Count <= 0)
    {
        return;
    }
    
    TotalGrenadeCount = FMath::Clamp(TotalGrenadeCount + Count, 0, MaxGrenadeCount);
}
