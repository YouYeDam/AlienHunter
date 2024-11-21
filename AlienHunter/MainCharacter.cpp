// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Gun.h"
#include "Sword.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"   
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = GetWorld()->GetAuthGameMode<AAlienHunterGameMode>();

	IsControlledByPlayer = IsPlayerControlled();

	if (IsControlledByPlayer)
	{
		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GameManager)
		{
			GunClass = GameManager->GetEquippedGun();
			SwordClass = GameManager->GetEquippedSword();

			InitializePlayerStats();
		}
	}

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

	if (!IsControlledByPlayer)
	{
		Gun->SetMeshVisibility(false);
	}

	CurrentHP = MaxHP;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMainCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Swing"), EInputEvent::IE_Pressed, this, &AMainCharacter::Swing);
	PlayerInputComponent->BindAction(TEXT("SwapGun"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwapGun);
	PlayerInputComponent->BindAction(TEXT("SwapSword"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwapSword);
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (!CanMove) {
		return;
	}
	AddMovementInput(GetActorRightVector() * AxisValue);
}

bool AMainCharacter::IsDead() const
{
	return CurrentHP <= 0;
}

bool AMainCharacter::IsAttacking() const
{
	return bIsAttacking;
}

bool AMainCharacter::SwitchUsingGun() const
{
	if (IsUsingGun) {
		return true;
	}
	else {
		return false;
	}
}

bool AMainCharacter::SwitchUsingSword() const
{
	if (IsUsingSword) {
		return true;
	}
	else {
		return false;
	}
}

void AMainCharacter::Shoot()
{
	if (Gun && IsUsingGun)
	{
		Gun->StartShoot();
	}
}

void AMainCharacter::Swing()
{
    // 점프 중인지 확인
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }

    if (Sword && IsUsingSword && !bIsAttacking && CanAttack)
    {
        Sword->StartSwing();
        bIsAttacking = true;
        CanAttack = false;
        CanMove = false;

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            Sword->EndSwing();
            bIsAttacking = false;

            FTimerHandle CanAttackTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(CanAttackTimerHandle, [this]()
            {
                CanAttack = true;
                CanMove = true;
            }, 0.7f, false);
        }, 0.4f, false); 
    }
}

void AMainCharacter::SwapGun()
{
	IsUsingGun = true;
	IsUsingSword = false;

	Gun->SetMeshVisibility(true);
	Sword->SetMeshVisibility(false);
}

void AMainCharacter::SwapSword()
{
	IsUsingGun = false;
	IsUsingSword = true;

	Gun->SetMeshVisibility(false);
	Sword->SetMeshVisibility(true);
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float TakedDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TakedDamageAmount = FMath::Min(CurrentHP, TakedDamageAmount);
	CurrentHP -= TakedDamageAmount;

	if (IsDead() && !IsAlreadyDead) {
		IsAlreadyDead = true;
		
		if (DeathSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
        }

		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UBoxComponent* BoxComp = FindComponentByClass<UBoxComponent>();
        if (BoxComp)
        {
            BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

		GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameManager && !IsControlledByPlayer)
		{
			int32 CurrentKillEnemyCount = GameManager->GetKillEnemyCount();
			GameManager->SetKillEnemyCount(CurrentKillEnemyCount++);
		}
	}

	return TakedDamageAmount;
}

void AMainCharacter::Heal(int32 HealAmount)
{
	CurrentHP += HealAmount;
	if (CurrentHP >= MaxHP) {
		CurrentHP = MaxHP;
	}
}

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

void AMainCharacter::InitializePlayerStats()
{
	if (GameManager)
	{
		MaxHP = GameManager->GetInitialHealth();
	}
}
