

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "GameManager.h"
#include "Gun.generated.h"

UCLASS()
class ALIENHUNTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh; // Gun 액터의 메시

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash; // 총구 이펙트

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect; // 명중 이펙트

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound; // 사격 사운드

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyAmmoSound; // 탄약 소진 시 사운드

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound; // 재장전 사운드

    UPROPERTY(EditAnywhere)
    UDataTable* GunDataTable; // 총기류 데이터 테이블

    UPROPERTY(EditAnywhere)
    int32 WeaponID; // 무기 ID

    UPROPERTY(EditAnywhere)
    bool bIsPlayerWeapon = false;

	UPROPERTY(EditAnywhere)
	float Damage = 10; // 사격 데미지

	UPROPERTY(EditAnywhere)
	float GunRange = 5000; // 사격 최대 거리

    UPROPERTY(EditAnywhere)
    int32 AmmoCount; // 탄약 개수

    UPROPERTY(EditAnywhere)
    int32 SpareAmmoCount; // 여분 탄약 개수

    UPROPERTY(EditAnywhere)
    float GunRecoil; // 반동 정도

	int32 MaxAmmoCount; // 최대 탄약 개수

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	void ApplyRecoil();

public:
	void StartShoot();
	void ReloadAmmo();
	void SetMeshVisibility(bool IsVisible);
	void InitializeWeaponData();

	int32 GetAmmoCount() const;
	int32 GetSpareAmmoCount() const;
};
