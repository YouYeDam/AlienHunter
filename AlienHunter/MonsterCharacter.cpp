#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentHP = MaxHP; // 현재 체력을 최대 체력으로 설정
}

// 헤드샷 사운드를 출력하는 메소드
void AMonsterCharacter::PlayHeadshotSound()
{
    if (HeadshotSound && !IsDead())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), HeadshotSound, GetActorLocation());
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
