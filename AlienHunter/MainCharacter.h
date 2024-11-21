// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	AAlienHunterGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> SwordClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY()
	ASword* Sword;

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;
	
	UPROPERTY(EditDefaultsOnly)
	int32 EXP = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 Energy = 0;

	int32 GainedEnergy = 0;
	int32 GainedEXP = 0;

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

	bool CanAttack = true;
	bool CanMove = true;
	bool IsControlledByPlayer;
	bool IsAlreadyDead = false;

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
