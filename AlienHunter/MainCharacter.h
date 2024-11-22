

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienHunterGameMode.h"
#include "GameManager.h"

#include "MainCharacter.generated.h"
class AGun;
class ASword;

UCLASS()
class ALIENHUNTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	AAlienHunterGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass; // 총기 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> SwordClass; // 도검 클래스

	UPROPERTY()
	AGun* Gun; // 총기 액터

	UPROPERTY()
	ASword* Sword; // 도검 액터

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound; // 죽음 사운드

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;
	
	UPROPERTY(EditDefaultsOnly)
	int32 EXP = 0; // 몬스터가 제공하는 경험치

	UPROPERTY(EditDefaultsOnly)
	int32 Energy = 0; // 몬스터가 제공하는 에너지

	int32 GainedEnergy = 0; // 게임 중 얻은 에너지
	int32 GainedEXP = 0; // 게임 중 얻은 경험치

	UPROPERTY()
	UGameManager* GameManager;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void InitializePlayerStats();

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingGun = true;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingSword = false;

	bool CanAttack = true; // 공격 가능 상태인지
	bool CanMove = true; // 이동 가능 상태인지
	bool IsControlledByPlayer; // 플레이어가 컨트롤하고 있는지
	bool IsAlreadyDead = false; // 이미 죽었는지

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;

	UFUNCTION(BlueprintPure)
	bool SwitchUsingGun() const;
	
	UFUNCTION(BlueprintPure)
	bool SwitchUsingSword() const;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Heal(int32 HealAmount);

	void Shoot();
	void Swing();
	void SwapGun();
	void SwapSword();

	int32 GetEnergy() const;
	int32 GetEXP() const;

	int32 GetGainedEnergy() const;
	void SetGainedEnergy(int32 NewEnergy);
	
	int32 GetGainedEXP() const;
	void SetGainedEXP(int32 NewEXP);
};
