

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "GameManager.h"
#include "GrenadeItemData.h"
#include "Grenade.generated.h"

class APlayerCharacter;

UCLASS()
class ALIENHUNTER_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrenade();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FTimerHandle ExplosionTimerHandle; // 폭발 타이머 핸들

	APlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect; // 폭발 이펙트

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExtraEffect; // 추가 이펙트

	UPROPERTY(EditAnywhere)
	USoundBase* GrenadeChargeSound; // 수류탄 차지 시 사운드

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound; // 폭발 사운드

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound; // 재장전 사운드

    UPROPERTY(EditAnywhere)
    UDataTable* GrenadeDataTable; // 수류탄류 데이터 테이블

	FGrenadeItemData* GrenadeData; // 수류탄 데이터

    UPROPERTY(EditAnywhere)
    int32 WeaponID; // 무기 ID

    UPROPERTY(EditAnywhere)
    bool bIsPlayerWeapon = false;

	UPROPERTY(EditAnywhere)
	float Damage = 100; // 수류탄 데미지

    UPROPERTY(EditAnywhere)
    int32 GrenadeCount; // 수류탄 개수

	UPROPERTY(EditAnywhere)
    int32 TotalGrenadeCount; // 여분 수류탄 개수

	UPROPERTY(EditAnywhere)
    int32 MaxGrenadeCount; // 최대 수류탄 개수

	UPROPERTY(EditAnywhere)
	float EffectRange; // 폭발 범위

	UPROPERTY(EditAnywhere)
	float SpecialEffectValue; // 특수효과 값

	UPROPERTY(EditAnywhere)
	float SpecialEffectDuration; // 특수효과 지속시간

	bool IsThrowing = false;
	
	AController* GetOwnerController() const;

	void Explode();

public:
	void SetMeshVisibility(bool bVisible);
	void InitializeWeaponData();

	bool ReloadGrenade();

	void ReadyExplosion();

	void PlayChargeSound();

	UStaticMeshComponent* GetMesh() const;

	int32 GetGrenadeCount() const;
	void SetGrenadeCount(int32 Count);

	int32 GetTotalGrenadeCount() const;
	void IncreaseTotalGrenadeCount(int32 Count);
};
