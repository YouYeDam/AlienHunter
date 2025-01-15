


#include "Gun.h"
#include "ShooterAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GunItemData.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
		
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

    // 플레이어 무기일 경우 데이터 초기화
    if (bIsPlayerWeapon && GunDataTable)
    {
        InitializeWeaponData();
    }
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	if (bIsPlayerWeapon)
	{
		MaxAmmoCount = AmmoCount;
	}
}

void AGun::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (bIsPlayerWeapon && GunDataTable)
    {
        InitializeWeaponData();
    }
}

// 무기 데이터 값을 받아 속성값을 초기화하는 메소드
void AGun::InitializeWeaponData()
{
    if (!GunDataTable || WeaponID <= 0)
    {
        return;
    }

    // DataTable에서 무기 데이터를 검색
    static const FString ContextString(TEXT("Weapon Data Initialization"));
    FGunItemData* GunData = GunDataTable->FindRow<FGunItemData>(FName(*FString::FromInt(WeaponID)), ContextString);

    if (GunData)
    {
        // 무기 속성 초기화
        Damage = GunData->ItemDamage;
        AmmoCount = GunData->AmmoCount;
        SpareAmmoCount = GunData->SpareAmmoCount;
        GunRange = GunData->GunRange;
        GunRecoil = GunData->GunRecoil;
    }
}

// 조건에 따라 메시의 가시성을 재설정하는 메소드
void AGun::SetMeshVisibility(bool IsVisible)
{
    if (Mesh)
    {
        Mesh->SetVisibility(IsVisible);
    }
}

// 사격 동작을 처리하는 메소드
void AGun::StartShoot()
{
	if (bIsPlayerWeapon && AmmoCount <= 0) // 플레이어는 탄약이 없으면 발사 불가
    {
		UGameplayStatics::SpawnSoundAttached(EmptyAmmoSound, Mesh, TEXT("MuzzleFlashSocket"));
        return;
    }

	if (bIsPlayerWeapon)
	{
		AmmoCount--; // 탄약 감소
		ApplyRecoil(); // 반동 적용
	}

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit; // 충돌 정보 구조체
	FVector ShotDirection; // 총알이 날아온 방향(발사 원점을 기준으로 하는 방향)
	bool IsSuccess = GunTrace(Hit, ShotDirection); // 충돌 여부 계산

	if (IsSuccess) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr) {
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr); // 데미지 정보 
			AController* OwnerController = GetOwnerController(); // 총기의 소유자

			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this); // 충돌한 액터에 데미지 적용
		}
	}
}

// 탄약을 재장전하는 메소드
void AGun::ReloadAmmo()
{
    if (SpareAmmoCount <= 0 || AmmoCount >= MaxAmmoCount) // 탄창이 없거나 이미 최대 탄약이면 리로드 불가
    {
        return;
    }

	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));

    int32 NeededAmmo = MaxAmmoCount - AmmoCount;
    int32 AmmoToReload = FMath::Min(NeededAmmo, SpareAmmoCount);

    AmmoCount += AmmoToReload;
    SpareAmmoCount -= AmmoToReload;
}

// 총알의 궤적을 계산하고 충돌 여부를 반환하는 메소드
bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) {
		return false; // 컨트롤러가 없으면 충돌 계산 실패
	}

	// 플레이어의 시점 위치와 회전 정보 가져오기
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector(); // 총알이 날아온 방향(발사 원점을 기준으로 반대 방향)을 설정

	FVector End = Location + Rotation.Vector() * GunRange; // 발사 끝점 계산

	// 충돌을 무시할 액터 설정
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	// 충돌 여부 계산
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params); // 여기서 Hit 설정
}

// 플레이어에게 총기 반동을 적용하는 메소드
void AGun::ApplyRecoil()
{
    AController* OwnerController = GetOwnerController();
    if (OwnerController)
    {
        // 현재 컨트롤러의 카메라 회전을 가져오기
        FRotator ControlRotation = OwnerController->GetControlRotation();

        // 반동값 추가
        float RecoilPitch = FMath::FRandRange(GunRecoil * 0.5f, GunRecoil); // 상하 반동
        float RecoilYaw = FMath::FRandRange(-GunRecoil * 0.6f, GunRecoil * 0.6f); // 좌우 반동

        ControlRotation.Pitch += RecoilPitch;
        ControlRotation.Yaw += RecoilYaw;

        // 새로운 컨트롤러 회전값 적용
        OwnerController->SetControlRotation(ControlRotation);
    }
}

// 총기의 소유자 컨트롤러를 반환하는 메소드
AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) {
		return nullptr;
	}
	return OwnerPawn->GetController();
}

int32 AGun::GetAmmoCount() const
{
	return AmmoCount;
}

int32 AGun::GetSpareAmmoCount() const
{
	return SpareAmmoCount;
}