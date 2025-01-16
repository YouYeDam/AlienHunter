


#include "PlayerCharacter.h"
#include "Gun.h"
#include "Sword.h"
#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Swing"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Swing);
	PlayerInputComponent->BindAction(TEXT("SwapGun"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwapGun);
	PlayerInputComponent->BindAction(TEXT("SwapSword"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwapSword);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorRightVector() * AxisValue);   
}

// 플레이어의 현재 무기가 총기류인지를 체크하는 메소드
bool APlayerCharacter::SwitchUsingGun() const
{
	if (IsUsingGun) {
		return true;
	}
	else {
		return false;
	}
}

// 플레이어의 현재 무기가 도검류인지를 체크하는 메소드
bool APlayerCharacter::SwitchUsingSword() const
{
	if (IsUsingSword) {
		return true;
	}
	else {
		return false;
	}
}

// 캐릭터가 공격 중인지를 체크하는 메소드
bool APlayerCharacter::IsAttacking() const
{
    return bIsAttacking;
}

// 캐릭터의 사격 메소드
void APlayerCharacter::Shoot()
{
	if (Gun && IsUsingGun)
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
	IsUsingGun = true;
	IsUsingSword = false;

	Gun->SetMeshVisibility(true);
	Sword->SetMeshVisibility(false);   
}

// 플레이어의 현재 무기를 도검류로 바꾸는 메소드
void APlayerCharacter::SwapSword()
{
	IsUsingGun = false;
	IsUsingSword = true;

	Gun->SetMeshVisibility(false);
	Sword->SetMeshVisibility(true);
}

// 상호작용 물체와 상호작용하는 메소드
void APlayerCharacter::Interact()
{
    // 플레이어가 상호작용할 수 있는 액터 찾기
    TArray<AActor*> OverlappingActors;
    GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        // MissionObject 태그가 있는 액터만 상호작용 처리
        if (Actor && Actor->ActorHasTag(FName("Mission Object")))
        {
            if (AInteractableActor* InteractableActor = Cast<AInteractableActor>(Actor))
            {
                InteractableActor->Interact(this);
                break; // 첫 번째 상호작용 가능한 액터에만 반응
            }
        }
    }
}

void APlayerCharacter::Reload()
{
    if (Gun && IsUsingGun)
    {
        Gun->ReloadAmmo();
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

int32 APlayerCharacter::GetGainedEnergy() const
{
    return GainedEnergy;
}

void APlayerCharacter::SetGainedEnergy(int32 NewEnergy)
{
    GainedEnergy += NewEnergy;
}

int32 APlayerCharacter::GetGainedEXP() const
{
    return GainedEXP;
}

void APlayerCharacter::SetGainedEXP(int32 NewEXP)
{
    GainedEXP = NewEXP;
}

AGun* APlayerCharacter::GetEquippedGun() const
{
	return Gun;
}

