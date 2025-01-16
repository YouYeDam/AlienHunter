


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

	CurrentHP -= TakedDamageAmount;

	 // 피격을 받고 죽은 경우에 대해 처리
	if (IsDead() && !IsAlreadyDead) {
		IsAlreadyDead = true; // 죽음이 중복되서 처리되는 경우 방지
		
		if (DeathSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
        }

		if (GameMode != nullptr) {
			GameMode->PawnKilled(this); // 게임 모드를 통해 폰의 죽음 처리
		}
		
		DetachFromControllerPendingDestroy(); // 컨트롤러 해제
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 캡슐 컴포넌트 해제

		UBoxComponent* BoxComp = FindComponentByClass<UBoxComponent>(); // 박스 컴포넌트 설정(모든 캐릭터가 가지고 있지는 않으므로)
        if (BoxComp)
        {
            BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 박스 컴포넌트 해제
        }

		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

		// 죽은 캐릭터가 적일 경우, 게임 매니저의 총 적 처치 수 증가
		if (GameManager && !IsControlledByPlayer)
		{
			int32 CurrentKillEnemyCount = GameManager->GetKillEnemyCount();
			GameManager->SetKillEnemyCount(CurrentKillEnemyCount++);
		}
	}

	return TakedDamageAmount;
}
