

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

class AGun;
class ASword;
class AGrenade;
class APerkEffector;
class AGrenadeEffector;

UCLASS()
class ALIENHUNTER_API APlayerCharacter : public AMainCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass; // 총기 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> SwordClass; // 도검 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGrenade> GrenadeClass; // 수류탄 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APerkEffector> PerkEffectorClass; // 퍽 이펙터 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGrenadeEffector> GrenadeEffectorClass; // 수류탄 이펙터 클래스

	UPROPERTY()
	AGun* Gun; // 총기 액터

	UPROPERTY()
	ASword* Sword; // 도검 액터

	UPROPERTY()
	AGrenade* Grenade; // 수류탄 액터

	UPROPERTY()
	APerkEffector* PerkEffector; // 퍽 이펙터 액터

	UPROPERTY()
	AGrenadeEffector* GrenadeEffector; // 수류탄 이펙터 액터

	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HealSound;

	int32 GainedEnergy = 0; // 게임 중 얻은 에너지
	int32 GainedEXP = 0; // 게임 중 얻은 경험치

	int32 HealKitCount = 2; // 회복 키트 개수
	const int MaxHealKitCount = 5; // 회복 키트 최대 소지 개수

	bool IsControlledByPlayer; // 플레이어가 컨트롤하고 있는지

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingGun = true;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingSword = false;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingGrenade = false;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsThrowing = false;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsGrenadeReady = true;

	bool CanAttack = true; // 공격 가능 상태인지
	bool CanCharge = true; // 투척 가능 상태인지
	bool CanMove = true; // 이동 가능 상태인지

	bool IsZooming = false; // 줌을 하고 있는 상태인지

	bool IsThrowingReady = false; // 수류탄 투척 준비 상태인지
	float ThrowChargeTime = 0.0f; // 수류탄 투척 차징 시간
	FTimerHandle ThrowChargeTimerHandle; // 수류탄 투척 차징 타이머

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetShieldPercent() const;

	void InitializePlayerStats();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

public:
	UFUNCTION(BlueprintPure)
	bool SwitchUsingGun() const;
	
	UFUNCTION(BlueprintPure)
	bool SwitchUsingSword() const;
	
	UFUNCTION(BlueprintPure)
	bool SwitchUsingGrenade() const;

	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;

	UFUNCTION(BlueprintPure)
	bool IsThrowing() const;

	UFUNCTION(BlueprintPure)
	bool IsGrenadeReady() const;

	void Heal(int32 HealAmount);

	void KillEnemy();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 플레이어의 입력에 동작
	virtual void Jump() override;
	void Shoot();
	void Swing();
	void SwapGun();
	void SwapSword();
	void SwapGrenade();
	void Interact();
	void Reload();
	void StopShoot();
	void UseHealKit();
	void ZoomIn();
	void ZoomOut();
	void ChargeThrowing();
	void ReleaseThrowing();

	int32 GetGainedEnergy() const;
	void SetGainedEnergy(int32 NewEnergy);
	
	int32 GetGainedEXP() const;
	void SetGainedEXP(int32 NewEXP);

	float GetMaxHP() const;
	void SetMaxHP(float NewHP, bool bMakeFullHP);

	float GetPlayerShield() const;
	void SetPlayerShield(float NewShield);

	int32 GetHealKitCount() const;
	void IncreaseHealKitCount(int32 HealKitAmount);

	AGun* GetEquippedGun() const;
	AGrenade* GetEquippedGrenade() const;

	APerkEffector* GetPerkEffector() const;

	AGrenadeEffector* GetGrenadeEffector() const;
};
