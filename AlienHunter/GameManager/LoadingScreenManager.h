

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingScreenManager.generated.h"

UCLASS()
class ALIENHUNTER_API ALoadingScreenManager : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    bool bIsMissionReady = false;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> LoadingScreenClass;

    UPROPERTY()
    UUserWidget* LoadingWidget;

    UPROPERTY()
    FName MissionLevel;

public:
    void WaitForAnyKey();
    void OnKeyPressed();
};
