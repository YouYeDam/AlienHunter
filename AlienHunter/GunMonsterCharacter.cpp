// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMonsterCharacter.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"

void AGunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 무기를 액터로 생성 후 플레이어의 WeaponSocket에 부착 및 소유자 설정
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (Gun) 
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}

    // 몬스터는 무기 외형이 필요 없으므로 숨기기
    Gun->SetMeshVisibility(false);
}

// 캐릭터가 공격 중인지를 체크하는 메소드
bool AGunMonsterCharacter::IsAttacking() const
{
    return bIsAttacking;
}

// 캐릭터의 사격 메소드
void AGunMonsterCharacter::Shoot()
{
    if (Gun)
	{
		Gun->StartShoot();
	}
}
