


#include "SwordMonsterCharacter.h"
#include "Sword.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASwordMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 무기를 액터로 생성 후 플레이어의 WeaponSocket에 부착 및 소유자 설정
	Sword = GetWorld()->SpawnActor<ASword>(SwordClass);
	if (Sword)
	{
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Sword->SetOwner(this);
		Sword->SetMeshVisibility(false);
	}

    // 몬스터는 무기 외형이 필요 없으므로 숨기기
    Sword->SetMeshVisibility(false);

    // AI의 회피 기능 강화
    GetCharacterMovement()->bUseRVOAvoidance = true;
    GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f;
    GetCharacterMovement()->AvoidanceWeight = 1.0f;
}

// 캐릭터가 공격 중인지를 체크하는 메소드
bool ASwordMonsterCharacter::IsAttacking() const
{
    return bIsAttacking;
}

// 캐릭터의 휘두르기 메소드
void ASwordMonsterCharacter::Swing()
{
    // 공중에 있다면 공격 불가
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
    {
        return;
    }

    AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(PlayerPawn);
    if (!PlayerCharacter)
    {
        return;
    }

    // 플레이어가 죽었다면 공격하지 않음
    if (PlayerCharacter->IsDead())
    {
        return;
    }

    // 플레이어가 공격 범위 밖이면 공격하지 않음
    float Dist = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
    
    if (Dist > AttackRange)
    {
        return;
    }

    if (Sword && !bIsAttacking && CanAttack)
    {
        Sword->StartSwing(); // 공격 중 상태 켜기
        bIsAttacking = true;
        CanAttack = false;
        CanMove = false; // 공격 중일 경우 이동 불가

		// 공격 중 상태 해제를 위해 타이머 설정
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
        {
            Sword->EndSwing(); // 공격 중 상태 끄기
            bIsAttacking = false;

			// 공격 후 이동 불가 상태 해제를 위해 타이머 설정
            FTimerHandle CanAttackTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(CanAttackTimerHandle, [this]()
            {
                CanAttack = true;
                CanMove = true;
            }, 0.7f, false); 
        }, 0.4f, false); //
    }
}

float ASwordMonsterCharacter::GetAttackRange() const
{
    return AttackRange;
}