


#include "MainCharacter.h"
#include "Gun.h"
#include "Sword.h"
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

	GameMode = GetWorld()->GetAuthGameMode<AAlienHunterGameMode>();

	IsControlledByPlayer = IsPlayerControlled();

	// 플레이어가 컨트롤 중이라면 현재 무기를 게임 매니저에서 장착중인 무기로 설정 및
	if (IsControlledByPlayer)
	{
		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GameManager)
		{
			GunClass = GameManager->GetEquippedGun();
			SwordClass = GameManager->GetEquippedSword();

			InitializePlayerStats(); // 플레이어의 스탯 초기 설정
		}
	}

	// 무기를 액터로 생성 후 플레이어의 WeaponSocket에 부착 및 소유자 설정
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (Gun) 
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}

	Sword = GetWorld()->SpawnActor<ASword>(SwordClass);
	if (Sword)
	{
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Sword->SetOwner(this);
		Sword->SetMeshVisibility(false);
	}

	// 플레이어가 컨트롤 중이 아니라면 메시를 가리기(적은 이미 스켈레톤에 무기 메시가 있으므로)
	if (!IsControlledByPlayer)
	{
		Gun->SetMeshVisibility(false);
	}

	CurrentHP = MaxHP; // 현재 체력을 최대 체력으로 설정
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 플레이어 입력을 바인딩하여 각 행동을 연결하는 메소드
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 입력
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);

	// 시점 조작 입력
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// 액션 입력
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMainCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Swing"), EInputEvent::IE_Pressed, this, &AMainCharacter::Swing);
	PlayerInputComponent->BindAction(TEXT("SwapGun"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwapGun);
	PlayerInputComponent->BindAction(TEXT("SwapSword"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwapSword);
}

// 플레이어가 앞뒤로 이동하는 메소드
void AMainCharacter::MoveForward(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// 플레이어가 좌우로 이동하는 메소드
void AMainCharacter::MoveRight(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// 캐릭터의 죽음 여부를 체크하는 메소드
bool AMainCharacter::IsDead() const
{
	return CurrentHP <= 0;
}

// 캐릭터가 공격 중인지를 체크하는 메소드
bool AMainCharacter::IsAttacking() const
{
	return bIsAttacking;
}

// 플레이어의 현재 무기가 총기류인지를 체크하는 메소드
bool AMainCharacter::SwitchUsingGun() const
{
	if (IsUsingGun) {
		return true;
	}
	else {
		return false;
	}
}

// 플레이어의 현재 무기가 도검류인지를 체크하는 메소드
bool AMainCharacter::SwitchUsingSword() const
{
	if (IsUsingSword) {
		return true;
	}
	else {
		return false;
	}
}

// 캐릭터의 사격 메소드
void AMainCharacter::Shoot()
{
	if (Gun && IsUsingGun)
	{
		Gun->StartShoot();
	}
}

// 캐릭터의 휘두르기 메소드
void AMainCharacter::Swing()
{
    // 공중에 있다면 공격 불가
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }

    if (Sword && IsUsingSword && !bIsAttacking && CanAttack)
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

// 플레이어의 현재 무기를 총기류로 바꾸는 메소드
void AMainCharacter::SwapGun()
{
	IsUsingGun = true;
	IsUsingSword = false;

	Gun->SetMeshVisibility(true);
	Sword->SetMeshVisibility(false);
}

// 플레이어의 현재 무기를 도검류로 바꾸는 메소드
void AMainCharacter::SwapSword()
{
	IsUsingGun = false;
	IsUsingSword = true;

	Gun->SetMeshVisibility(false);
	Sword->SetMeshVisibility(true);
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

// 캐릭터의 치유 효과를 담당하는 메소드
void AMainCharacter::Heal(int32 HealAmount)
{
	CurrentHP += HealAmount;
	if (CurrentHP >= MaxHP) {
		CurrentHP = MaxHP;
	}
}

// 플레이어의 초기 스탯을 설정하는 메소드
void AMainCharacter::InitializePlayerStats()
{
	if (GameManager)
	{
		MaxHP = GameManager->GetInitialHealth();
	}
}

// 프로그레스바 UI 표시를 위한 체력 비율 계산
float AMainCharacter::GetHealthPercent() const
{
	return CurrentHP / MaxHP;
}

int32 AMainCharacter::GetEXP() const
{
	return EXP;
}

int32 AMainCharacter::GetEnergy() const
{
	return Energy;
}

int32 AMainCharacter::GetGainedEnergy() const
{
	return GainedEnergy;
}

void AMainCharacter::SetGainedEnergy(int32 NewEnergy)
{
	GainedEnergy += NewEnergy;
}

int32 AMainCharacter::GetGainedEXP() const
{
	return GainedEXP;
}

void AMainCharacter::SetGainedEXP(int32 NewEXP)
{
	GainedEXP = NewEXP;
}
