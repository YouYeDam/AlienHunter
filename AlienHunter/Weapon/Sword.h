

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Sword.generated.h"

UCLASS()
class ALIENHUNTER_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	ASword();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsSwinging = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundBase* SwingSound; // 휘두르기 사운드

    UPROPERTY(EditAnywhere)
    UDataTable* SwordDataTable; // 총기류 데이터 테이블

    UPROPERTY(EditAnywhere)
    int32 WeaponID; // 무기 ID

    UPROPERTY(EditAnywhere)
    bool bIsPlayerWeapon = false;

	UPROPERTY(EditAnywhere)
	float Damage = 0;

public:
	void SetMeshVisibility(bool bVisible);
	void StartSwing(); // 공격 시작
	void EndSwing();   // 공격 종료

	void InitializeWeaponData();

	int32 GetDamage() const;
	void SetDamage(int32 NewDamage);
};
