

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "GameManager.h"
#include "Grenade.generated.h"

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
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    UDataTable* GrenadeDataTable; // 수류탄류 데이터 테이블

    UPROPERTY(EditAnywhere)
    int32 WeaponID; // 무기 ID

    UPROPERTY(EditAnywhere)
    bool bIsPlayerWeapon = false;

	UPROPERTY(EditAnywhere)
	float Damage = 100; // 수류탄 데미지

    UPROPERTY(EditAnywhere)
    int32 GrenadeCount; // 수류탄 개수

	UPROPERTY(EditAnywhere)
	float EffectRange; // 폭발 범위

	UPROPERTY(EditAnywhere)
	float SpecialEffectValue; // 특수효과 값

	bool IsThrowing = false;
	
public:
	void SetMeshVisibility(bool bVisible);
	
	void InitializeWeaponData();

	int32 GetGrenadeCount() const;
};
