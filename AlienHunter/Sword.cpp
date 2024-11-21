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

void ASword::SetMeshVisibility(bool bVisible)
{
	if (Mesh)
    {
        Mesh->SetVisibility(bVisible);
    }
}

void ASword::StartSwing()
{
	if (IsSwinging) {
		return;
	}
	
	IsSwinging = true;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 활성화

	UGameplayStatics::SpawnSoundAttached(SwingSound, Mesh, TEXT("SwingSoundSocket"));
}

void ASword::EndSwing()
{
	IsSwinging = false;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
}

void ASword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsSwinging && OtherActor && OtherActor != GetOwner())
    {
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


