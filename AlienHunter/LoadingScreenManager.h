

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadingScreenManager.generated.h"

UCLASS()
class ALIENHUNTER_API ALoadingScreenManager : public AActor
{
    GENERATED_BODY()
    
private:
    bool bIsMissionReady = false;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> LoadingScreenClass;

    UPROPERTY()
    UUserWidget* LoadingWidget;

    UPROPERTY()
    FName MissionName;

public:
    virtual void BeginPlay() override;

    void WaitForAnyKey();
    void OnKeyPressed();
};
