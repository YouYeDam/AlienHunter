// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "SwordMonsterCharacter.generated.h"

class ASword;
UCLASS()
class ALIENHUNTER_API ASwordMonsterCharacter : public AMonsterCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASword> SwordClass; // 도검 클래스
	
	UPROPERTY()
	ASword* Sword; // 도검 액터

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;

	bool CanAttack = true; // 공격 가능 상태인지
	bool CanMove = true; // 이동 가능 상태인지

	float AttackRange = 100.0f;

public:
	UFUNCTION(BlueprintPure)
	bool IsAttacking() const;

	void Swing();

	float GetAttackRange() const;
};
