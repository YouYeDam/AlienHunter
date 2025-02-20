
#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Components/SphereComponent.h"
#include "MonsterCharacter.generated.h"

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

public:
	int32 GetEnergy() const;
	int32 GetEXP() const;
	
	void PlayHeadShotEffect(const FVector& HitLocation, const FRotator& ShotDirection);

	USphereComponent* GetHeadshotHitbox() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
