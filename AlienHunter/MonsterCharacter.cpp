#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentHP = MaxHP; // 현재 체력을 최대 체력으로 설정
}

// 헤드샷 이펙트를 출력하는 메소드
void AMonsterCharacter::PlayHeadShotEffect(const FVector& HitLocation, const FRotator& ShotDirection)
{
    if (HeadShotEffect && !IsDead())
    {
        UE_LOG(LogTemp, Warning, TEXT("헤드샷"));
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HeadShotEffect, HitLocation, ShotDirection);
    }
}

int32 AMonsterCharacter::GetEnergy() const
{
    return Energy;
}

int32 AMonsterCharacter::GetEXP() const
{
    return EXP;
}

USphereComponent* AMonsterCharacter::GetHeadshotHitbox() const
{
    return FindComponentByClass<USphereComponent>();
}