


#include "Gun.h"
#include "ShooterAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
		
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	FVector End = Location + Rotation.Vector() * MaxRange; // 발사 끝점 계산

	// 충돌을 무시할 액터 설정
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	// 충돌 여부 계산
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params); // 여기서 Hit 설정
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
