


#include "CollectObjectGameMode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void ACollectObjectGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // "Mission Object" 태그를 가진 모든 액터를 카운트
    TArray<AActor*> MissionObjectActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Mission Object"), MissionObjectActors);

    RequiredCollectingCount = FMath::CeilToInt(MissionObjectActors.Num() * 0.7);
}

void ACollectObjectGameMode::ItemCollected()
{
        CurrentCollectingCount++;
        UpdateHUDMissionProgress();

        if (CurrentCollectingCount >= RequiredCollectingCount)
        {
            EndGame(true);
        }
}

int32 ACollectObjectGameMode::GetCurrentCollectingCount() const
{
    return CurrentCollectingCount;
}

int32 ACollectObjectGameMode::GetRequiredCollectingCount() const
{
    return RequiredCollectingCount;
}
