


#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"   
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AAlienHunterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	IsControlledByPlayer = IsPlayerControlled();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 캐릭터의 죽음 여부를 체크하는 메소드
bool AMainCharacter::IsDead() const
{
	return CurrentHP <= 0;
}

// 캐릭터의 피격을 구현한 메소드
float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float TakedDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // 데미지 양 계산
	TakedDamageAmount = FMath::Min(CurrentHP, TakedDamageAmount); // 남아있는 체력보다 피격 데미지가 커지지 않도록(음수값 방지)

    float RemainingDamage = TakedDamageAmount; // 임시 변수로 남은 데미지 추적

    // 쉴드가 있는 경우, 먼저 차감
    if (CurrentShield > 0)
    {
        if (CurrentShield >= RemainingDamage) 
        {
            CurrentShield -= RemainingDamage;
            RemainingDamage = 0; // 쉴드가 모든 데미지를 흡수함
        }
        else 
        {
            RemainingDamage -= CurrentShield; // 쉴드가 일부 데미지만 막음
            CurrentShield = 0;
        }
    }

    // 남은 데미지를 체력에서 차감
    CurrentHP -= RemainingDamage;
    if (CurrentHP < 0)
    {
        CurrentHP = 0; // 체력이 음수가 되는 것 방지
    }

    // 사망 체크 및 처리
    if (IsDead())
    {
        HandleDeath();
    }

	return TakedDamageAmount;
}

void AMainCharacter::HandleDeath()
{
    if (IsAlreadyDead) return;

    IsAlreadyDead = true; // 중복 처리 방지

    if (DeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
    }

    if (GameMode)
    {
        GameMode->PawnKilled(this);
    }

    DetachFromControllerPendingDestroy(); // 컨트롤러 해제

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 해제

    // 박스 컴포넌트가 존재하는 경우 충돌 해제
    UBoxComponent* BoxComp = FindComponentByClass<UBoxComponent>();
    if (BoxComp)
    {
        BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // 게임 매니저에서 적 처치 수 증가
    if (GameManager && !IsControlledByPlayer)
    {
        int32 KillCount = GameManager->GetKillEnemyCount();
        GameManager->SetKillEnemyCount(KillCount + 1);
    }

    // 몬스터일 경우 10초 뒤 자동 제거
    if (!IsControlledByPlayer)
    {
        SetLifeSpan(10.0f);
    }
}