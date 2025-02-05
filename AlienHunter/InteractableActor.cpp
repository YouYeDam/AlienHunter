// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "CollectObjectGameMode.h"
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

void AInteractableActor::Interact(AActor* InteractingActor)
{
    if (bCanInteract) // 플레이어가 범위 내에 있을 때만
    {
        Collect(InteractingActor);
    }	
}

void AInteractableActor::Collect(AActor* Collector)
{
    // 게임모드에 수집 이벤트 전달
    if (ACollectObjectGameMode* GameMode = Cast<ACollectObjectGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->ItemCollected();
    }

    // 아이템 파괴
    Destroy();
}
