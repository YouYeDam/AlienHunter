#include "Sword.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlapBegin);

}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 조건에 따라 메시의 가시성을 재설정하는 메소드
void ASword::SetMeshVisibility(bool bVisible)
{
	if (Mesh)
    {
        Mesh->SetVisibility(bVisible);
    }
}

// 휘두르기 동작의 시작을 처리하는 메소드
void ASword::StartSwing()
{
	if (IsSwinging) {
		return;
	}
	
	IsSwinging = true;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 활성화

	UGameplayStatics::SpawnSoundAttached(SwingSound, Mesh, TEXT("SwingSoundSocket"));
}

// 휘두르기 동작의 끝을 처리하는 메소드
void ASword::EndSwing()
{
	IsSwinging = false;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
}

// 근접 공격의 충돌 오버랩을 감지하여 적에게 데미지를 입히는 메서드
void ASword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsSwinging && OtherActor && OtherActor != GetOwner())
    {
        // 태그를 이용하여 같은 태그를 가진 대상이 피해를 입지 못하도록 설정
        if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Player")))
        {
            FVector HitDirection = (OtherActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
            FPointDamageEvent DamageEvent(Damage, SweepResult, HitDirection, nullptr);

            AController* OwnerController = GetInstigatorController();
            OtherActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
        }
        else if (GetOwner()->ActorHasTag(TEXT("Player")) && OtherActor->ActorHasTag(TEXT("Enemy")))
        {
            FVector HitDirection = (OtherActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
            FPointDamageEvent DamageEvent(Damage, SweepResult, HitDirection, nullptr);

            AController* OwnerController = GetInstigatorController();
            OtherActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
        }
    }
}


