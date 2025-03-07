


#include "MonsterCharacter.h"
#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Components/CapsuleComponent.h"

void AMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentHP = MaxHP; // 현재 체력을 최대 체력으로 설정

    // 블랙보드에 로밍 여부 설정
    ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
    if (AIController && AIController->GetBlackboardComponent())
    {
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsRoaming"), bIsRoaming);
    }
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

    OnMonsterDamaged.Broadcast(); 
    
    // 주변 몬스터들에게 전투 상태 전파
    LinkNearbyMonsters();

    return TakedDamageAmount;
}

// 몬스터 피격 시 주변 몬스터도 전투 상태에 돌입하는 링크 시스템을 활성화하는 메소드
void AMonsterCharacter::LinkNearbyMonsters()
{
    if (!GetWorld())
    {
        return;
    }

    FVector CurrentLocation = GetActorLocation();

    TArray<AActor*> FoundMonsters;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Enemy", FoundMonsters);

    for (AActor* Actor : FoundMonsters)
    {
        AMonsterCharacter* NearbyMonster = Cast<AMonsterCharacter>(Actor);
        if (NearbyMonster && NearbyMonster != this) // 자기 자신 제외
        {
            float Distance = FVector::Dist(CurrentLocation, NearbyMonster->GetActorLocation());

            if (Distance <= LinkRange) // 반경 내 몬스터만 적용
            {
                ABaseAIController* NearbyAIController = Cast<ABaseAIController>(NearbyMonster->GetController());
                if (NearbyAIController)
                {
                    NearbyAIController->SetInCombat(true);
                }

                // 주변 몬스터도 OnMonsterDamaged 이벤트 발생
                NearbyMonster->OnMonsterDamaged.Broadcast();
            }
        }
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

bool AMonsterCharacter::GetIsRoaming() const
{
    return bIsRoaming;
}

USphereComponent* AMonsterCharacter::GetHeadshotHitbox() const
{
    return FindComponentByClass<USphereComponent>();
}

