

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionSummaryManager.generated.h"

UCLASS()
class ALIENHUNTER_API AMissionSummaryManager : public AActor
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> MissionSummaryClass;

    UPROPERTY()
    UUserWidget* MissionSummaryWidget;

};
