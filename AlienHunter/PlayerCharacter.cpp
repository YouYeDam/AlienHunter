


#include "PlayerCharacter.h"
#include "Gun.h"
#include "Sword.h"
#include "Grenade.h"
#include "InteractableActor.h"
#include "PerkData.h"
#include "PerkEffector.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainPlayerController.h"

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    IsControlledByPlayer = IsPlayerControlled();

	// 플레이어가 컨트롤 중이라면 현재 무기를 게임 매니저에서 장착중인 무기로 설정
	if (IsControlledByPlayer)
	{
		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GameManager)
		{
			GunClass = GameManager->GetEquippedGun();
			SwordClass = GameManager->GetEquippedSword();
            GrenadeClass = GameManager->GetEquippedGrenade();

			InitializePlayerStats(); // 플레이어의 스탯 초기 설정
		}
	}
	
	CurrentHP = MaxHP; // 현재 체력을 최대 체력으로 설정

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

    Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
	if (Grenade)
	{
		Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GrenadeSocket"));
		Grenade->SetOwner(this);
		Grenade->SetMeshVisibility(false);
	}

    // FollowCamera 가져오기
    FollowCamera = FindComponentByClass<UCameraComponent>();

    // 퍽 적용
    if (GameManager)
    {
        const TArray<FPerkData>& PlayerPerks = GameManager->GetChosenPerks(); // 게임 매니저에서 퍽 데이터 가져옴

        if (PlayerPerks.Num() > 0)
        {
            PerkEffector = GetWorld()->SpawnActor<APerkEffector>(PerkEffectorClass);
            if (PerkEffector)
            {
                PerkEffector->ApplyPerks(this, PlayerPerks);
            }
        }
    }
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 입력
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);

	// 시점 조작 입력
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// 액션 입력
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Swing"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Swing);
	PlayerInputComponent->BindAction(TEXT("SwapGun"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwapGun);
	PlayerInputComponent->BindAction(TEXT("SwapSword"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwapSword);
    PlayerInputComponent->BindAction(TEXT("SwapGrenade"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwapGrenade);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &APlayerCharacter::StopShoot);
    PlayerInputComponent->BindAction(TEXT("UseHealKit"), EInputEvent::IE_Pressed, this, &APlayerCharacter::UseHealKit);
    PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ZoomIn);
    PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Released, this, &APlayerCharacter::ZoomOut);
}

void APlayerCharacter::Jump()
{
    // 땅에 있을 때만 점프 사운드 실행
    if (GetCharacterMovement() && GetCharacterMovement()->IsMovingOnGround())
    {
        if (JumpSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
        }
    }

    Super::Jump();
}


void APlayerCharacter::MoveForward(float AxisValue)
{
	if (!CanMove) 
    {
		return;
	}
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if (!CanMove) 
    {
		return;
	}
	AddMovementInput(GetActorRightVector() * AxisValue);   
}

// 플레이어의 현재 무기가 총기류인지를 체크하는 메소드
bool APlayerCharacter::SwitchUsingGun() const
{
	if (IsUsingGun) 
    {
		return true;
	}
	else 
    {
		return false;
	}
}

// 플레이어의 현재 무기가 도검류인지를 체크하는 메소드
bool APlayerCharacter::SwitchUsingSword() const
{
	if (IsUsingSword) 
    {
		return true;
	}
	else 
    {
		return false;
	}
}

// 플레이어의 현재 무기가 도검류인지를 체크하는 메소드
bool APlayerCharacter::SwitchUsingGrenade() const
{
	if (IsUsingGrenade) 
    {
		return true;
	}
	else 
    {
		return false;
	}
}

// 캐릭터가 공격 중인지를 체크하는 메소드
bool APlayerCharacter::IsAttacking() const
{
    return bIsAttacking;
}

bool APlayerCharacter::IsThrowing() const
{
    return bIsThrowing;
}

void APlayerCharacter::Heal(int32 HealAmount)
{
    if (HealSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HealSound, GetActorLocation());
    }

    if (CurrentHP + HealAmount > MaxHP)
    {
        CurrentHP = MaxHP;
    }
    else
    {
        CurrentHP += HealAmount;
    }
}

// 캐릭터의 사격 메소드
void APlayerCharacter::Shoot()
{
    if (Gun && IsUsingGun && Gun->GetBarrage()) // 연발 가능 여부 체크
    {
        Gun->StartBarrage();
    }
    else if (Gun && IsUsingGun)
    {
        Gun->StartShoot();
    }
}

// 캐릭터의 휘두르기 메소드
void APlayerCharacter::Swing()
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
void APlayerCharacter::SwapGun()
{
    if (IsUsingGun)
    {
        return;
    }

	IsUsingGun = true;
	IsUsingSword = false;
    IsUsingGrenade = false;

	Gun->SetMeshVisibility(true);
	Sword->SetMeshVisibility(false);
    Grenade->SetMeshVisibility(false);
}

// 플레이어의 현재 무기를 도검류로 바꾸는 메소드
void APlayerCharacter::SwapSword()
{
    if (IsZooming || IsUsingSword)
    {
        return;
    }
	IsUsingGun = false;
	IsUsingSword = true;
    IsUsingGrenade = false;

	Gun->SetMeshVisibility(false);
	Sword->SetMeshVisibility(true);
    Grenade->SetMeshVisibility(false);
}

// 플레이어의 현재 무기를 수류탄류로 바꾸는 메소드
void APlayerCharacter::SwapGrenade()
{
    if (IsZooming || IsUsingGrenade)
    {
        return;
    }

	IsUsingGun = false;
	IsUsingSword = false;
    IsUsingGrenade = true;

	Gun->SetMeshVisibility(false);
	Sword->SetMeshVisibility(false);
    Grenade->SetMeshVisibility(true);
}

// 상호작용 물체와 상호작용하는 메소드
void APlayerCharacter::Interact()
{
    // 플레이어가 상호작용할 수 있는 액터 찾기
    TArray<AActor*> OverlappingActors;
    GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        // AInteractableActor 클래스인지 확인
        if (AInteractableActor* InteractableActor = Cast<AInteractableActor>(Actor))
        {
            InteractableActor->Interact(this);
            break; // 첫 번째 상호작용 가능한 액터에만 반응
        }
    }
}

// 탄약을 재장전하는 메소드
void APlayerCharacter::Reload()
{
    if (Gun && IsUsingGun)
    {
        Gun->ReloadAmmo();
    }
}

// 총기 발사를 멈추는 메소드
void APlayerCharacter::StopShoot()
{
    if (!Gun || !IsUsingGun || !Gun->GetBarrage())
    {
        return;
    }

    Gun->StopBarrage();
}

// 회복 키트를 사용하는 메소드
void APlayerCharacter::UseHealKit()
{
    if (HealKitCount <= 0)
    {
        return;
    }

    int32 HealAmount = MaxHP * 0.3;
    Heal(HealAmount);
    HealKitCount--;
}

// 총기 줌 인을 하는 메소드
void APlayerCharacter::ZoomIn()
{
    if (!Gun || !IsUsingGun || !Gun->GetZoom())
    {
        return;
    }

    if (!IsZooming)
    {
        IsZooming = true;
        if (FollowCamera)
        {
            FollowCamera->SetFieldOfView(10.0f);
        }
    }
}

// 총기 줌 아웃을 하는 메소드
void APlayerCharacter::ZoomOut()
{
    if (!Gun || !IsUsingGun || !Gun->GetZoom())
    {
        return;
    }

    if (IsZooming)
    {
        IsZooming = false;
        if (FollowCamera)
        {
            FollowCamera->SetFieldOfView(90.0f);
        }
    }
}

// 적 처치 시 발생하는 메소드
void APlayerCharacter::KillEnemy()
{
    if (PerkEffector) 
    {
        PerkEffector->TriggerTacticalRush();
    }
}

// 플레이어의 초기 스탯을 설정하는 메소드
void APlayerCharacter::InitializePlayerStats()
{
	if (GameManager)
	{
		MaxHP = GameManager->GetInitialHealth();
	}
}

// 프로그레스바 UI 표시를 위한 체력 비율 계산
float APlayerCharacter::GetHealthPercent() const
{
    return CurrentHP / MaxHP;
}

// 프로그레스바 UI 표시를 위한 쉴드 비율 계산
float APlayerCharacter::GetShieldPercent() const
{
    return CurrentShield / MaxHP;
}

int32 APlayerCharacter::GetGainedEnergy() const
{
    return GainedEnergy;
}

void APlayerCharacter::SetGainedEnergy(int32 NewEnergy)
{
    GainedEnergy = NewEnergy;
}

int32 APlayerCharacter::GetGainedEXP() const
{
    return GainedEXP;
}

void APlayerCharacter::SetGainedEXP(int32 NewEXP)
{
    GainedEXP = NewEXP;
}

float APlayerCharacter::GetMaxHP() const
{
	return MaxHP;
}

void APlayerCharacter::SetMaxHP(float NewHP, bool bMakeFullHP)
{
	MaxHP = NewHP;

    if (bMakeFullHP)
    {
        CurrentHP = MaxHP;
    }
}

float APlayerCharacter::GetPlayerShield() const
{
    return CurrentShield;
}

void APlayerCharacter::SetPlayerShield(float NewShield)
{
    CurrentShield = NewShield;
}

int32 APlayerCharacter::GetHealKitCount() const
{
    return HealKitCount;
}

void APlayerCharacter::IncreaseHealKitCount(int32 HealKitAmount)
{
    HealKitCount = FMath::Min(HealKitCount + HealKitAmount, MaxHealKitCount);
}

AGun* APlayerCharacter::GetEquippedGun() const
{
	return Gun;
}

APerkEffector* APlayerCharacter::GetPerkEffector() const
{
    return PerkEffector;
}