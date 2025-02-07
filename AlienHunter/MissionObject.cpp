


#include "MissionObject.h"
#include "CollectObjectGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMissionObject::Collect(AActor* Collector)
{
    Super::Collect(Collector);
    
    // 게임모드에 수집 이벤트 전달
    if (ACollectObjectGameMode* GameMode = Cast<ACollectObjectGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->ItemCollected();
    }

    // 아이템 파괴
    Destroy();
}
