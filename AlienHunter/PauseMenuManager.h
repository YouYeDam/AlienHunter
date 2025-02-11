

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PauseMenuManager.generated.h"

UCLASS()
class ALIENHUNTER_API APauseMenuManager : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	APlayerController* PlayerController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> HelpMenuClass;

    UPROPERTY()
    UUserWidget* CurrentWidget; // 현재 활성화된 메뉴 위젯

public:
	virtual void BeginPlay() override;

    void ShowPauseMenu();

    void ShowHelpMenu();

    void CloseAllMenu();
};
