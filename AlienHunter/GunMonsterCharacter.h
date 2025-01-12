// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "GunMonsterCharacter.generated.h"

class AGun;
UCLASS()
class ALIENHUNTER_API AGunMonsterCharacter : public AMonsterCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass; // 총기 클래스

	UPROPERTY()
	AGun* Gun; // 총기 액터

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	bool CanAttack = true; // 공격 가능 상태인지
	bool CanMove = true; // 이동 가능 상태인지

public:
	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;

	void Shoot();
};
