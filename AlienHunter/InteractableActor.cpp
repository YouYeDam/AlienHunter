


#include "InteractableActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(Mesh);

	// 상호작용 범위 설정
	InteractionSphere->SetSphereRadius(InteractableRadius);

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnSphereBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnSphereEndOverlap);
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        bCanInteract = true; // 상호작용 가능 상태
    }
}

void AInteractableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        bCanInteract = false; // 상호작용 불가 상태
    }
}

// 상호작용 시 아이템 수집을 호출하는 메소드
void AInteractableActor::Interact(AActor* InteractingActor)
{
    if (bCanInteract) // 플레이어가 범위 내에 있을 때만
    {
        Collect(InteractingActor);
    }	
}

// 아이템 수집 시 실행하는 메소드(인터페이스용 -> 자식에서 구현)
void AInteractableActor::Collect(AActor* Collector)
{
    // No Action.
}
