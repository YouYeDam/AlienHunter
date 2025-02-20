#include "Sword.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "SwordItemData.h"

ASword::ASword()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Mesh);
	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnOverlapBegin);

    // 플레이어 무기일 경우 데이터 초기화
    if (bIsPlayerWeapon && SwordDataTable)
    {
        InitializeWeaponData();
    }
}

// 에디터에서 SwordItemData의 값을 즉시 반영하게 하는 메소드
#if WITH_EDITOR
void ASword::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 변경된 속성 확인
    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
                             ? PropertyChangedEvent.Property->GetFName()
                             : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(ASword, SwordDataTable) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(ASword, WeaponID))
    {
        // SwordDataTable 또는 WeaponID가 변경된 경우 처리
        if (bIsPlayerWeapon && SwordDataTable)
        {
            InitializeWeaponData(); // 데이터 초기화 함수 호출
        }
    }
}
#endif

void ASword::BeginPlay()
{
	Super::BeginPlay();
}

void ASword::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (bIsPlayerWeapon && SwordDataTable)
    {
        InitializeWeaponData(); // 데이터 초기화
    }
}

// 무기 데이터 값을 받아 속성값을 초기화하는 메소드
void ASword::InitializeWeaponData()
{
    if (!SwordDataTable || WeaponID <= 0)
    {
        return;
    }

    // DataTable에서 무기 데이터를 검색
    static const FString ContextString(TEXT("Weapon Data Initialization"));
    FSwordItemData* SwordData = SwordDataTable->FindRow<FSwordItemData>(FName(*FString::FromInt(WeaponID)), ContextString);

    if (SwordData)
    {
        // 무기 속성 초기화
        Damage = SwordData->ItemDamage;
    }
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
        bool bIsOwnerEnemy = GetOwner()->ActorHasTag(TEXT("Enemy"));
        bool bIsOwnerPlayer = GetOwner()->ActorHasTag(TEXT("Player"));
        bool bIsTargetEnemy = OtherActor->ActorHasTag(TEXT("Enemy"));
        bool bIsTargetPlayer = OtherActor->ActorHasTag(TEXT("Player"));

        // 같은 태그를 가진 대상은 피해를 입지 않음
        if ((bIsOwnerEnemy && bIsTargetEnemy) || (bIsOwnerPlayer && bIsTargetPlayer))
        {
            return;
        }

        FVector HitDirection = (OtherActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
        FPointDamageEvent DamageEvent(Damage, SweepResult, HitDirection, nullptr);

        AController* OwnerController = GetInstigatorController();
        OtherActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
    }
}



