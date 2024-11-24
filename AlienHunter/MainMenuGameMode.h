

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"


UCLASS()
class ALIENHUNTER_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;
};
