

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienHunterGameMode.h"
#include "GameManager.h"

#include "MainCharacter.generated.h"

UCLASS()
class ALIENHUNTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere)
	float CurrentHP;

	UPROPERTY()
	UGameManager* GameManager;
	
private:
	AAlienHunterGameMode* GameMode;

	bool IsControlledByPlayer; // 플레이어가 컨트롤하고 있는지

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound; // 죽음 사운드

	bool IsAlreadyDead = false; // 이미 죽었는지

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
