
#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "MonsterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterDamaged); // 몬스터 피격 시 활용할 델리게이트 선언

UCLASS()
class ALIENHUNTER_API AMonsterCharacter : public AMainCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 EXP = 0; // 몬스터가 제공하는 경험치

	UPROPERTY(EditAnywhere)
	int32 Energy = 0; // 몬스터가 제공하는 에너지

private:
    UPROPERTY(EditAnywhere)
    UParticleSystem* HeadShotEffect; // 헤드샷 이펙트

	UPROPERTY(EditAnywhere)
	bool bIsRoaming = false; // 몬스터가 로밍을 하는지 여부

	float LinkRange = 500.0f; // 링크 범위

public:
	int32 GetEnergy() const;
	int32 GetEXP() const;
	bool GetIsRoaming() const;
	
	void PlayHeadShotEffect(const FVector& HitLocation, const FRotator& ShotDirection);

	USphereComponent* GetHeadshotHitbox() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void LinkNearbyMonsters();
	
    UPROPERTY()
    FOnMonsterDamaged OnMonsterDamaged;
};
