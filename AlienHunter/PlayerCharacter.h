// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "PlayerCharacter.generated.h"

class AGun;
class ASword;

UCLASS()
class ALIENHUNTER_API APlayerCharacter : public AMainCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass; // 총기 클래스

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> SwordClass; // 도검 클래스

	UPROPERTY()
	AGun* Gun; // 총기 액터

	UPROPERTY()
	ASword* Sword; // 도검 액터

	int32 GainedEnergy = 0; // 게임 중 얻은 에너지
	int32 GainedEXP = 0; // 게임 중 얻은 경험치

	bool IsControlledByPlayer; // 플레이어가 컨트롤하고 있는지

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingGun = true;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool IsUsingSword = false;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void InitializePlayerStats();

	bool CanAttack = true; // 공격 가능 상태인지
	bool CanMove = true; // 이동 가능 상태인지

public:
	UFUNCTION(BlueprintPure)
	bool SwitchUsingGun() const;
	
	UFUNCTION(BlueprintPure)
	bool SwitchUsingSword() const;
	
	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Shoot();
	void Swing();
	void SwapGun();
	void SwapSword();
	void Interact();
	void Reload();
	
	int32 GetGainedEnergy() const;
	void SetGainedEnergy(int32 NewEnergy);
	
	int32 GetGainedEXP() const;
	void SetGainedEXP(int32 NewEXP);

	AGun* GetEquippedGun() const;
};
