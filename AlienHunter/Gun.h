

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class ALIENHUNTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh; // Gun 액터의 메시

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash; // 총구 이펙트

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect; // 명중 이펙트

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound; // 사격 사운드

	UPROPERTY(EditAnywhere)
	float MaxRange = 5000; // 사격 최대 거리

	UPROPERTY(EditAnywhere)
	float Damage = 10; // 사격 데미지

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

public:
	void StartShoot();
	void SetMeshVisibility(bool IsVisible);
};
