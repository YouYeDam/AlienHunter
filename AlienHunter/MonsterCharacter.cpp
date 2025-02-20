#include "MonsterCharacter.h"
#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Components/CapsuleComponent.h"

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
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HeadShotEffect, HitLocation, ShotDirection);
    }
}

// 몬스터 피격 시 전투 상태를 활성화하는 메소드
float AMonsterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float TakedDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
    if (AIController)
    {
        AIController->SetInCombat(true);
    }

    return TakedDamageAmount;
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

