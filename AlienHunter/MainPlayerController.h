

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameManager.h"
#include "HUDWidget.h"
#include "PauseMenuWidget.h"
#include "MainPlayerController.generated.h"

class APauseMenuManager;

UCLASS()
class ALIENHUNTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY()
	UGameManager* GameManager;

	UPROPERTY(EditAnywhere)
	float GameEndDelay = 5; // 게임 종료 후 레벨 로드 딜레이 시간

	FTimerHandle GameEndTimer; // 게임 종료 후 레벨 로드 타이머

	float BonusLootMultiplier = 1.0f; // 보너스 보상 배율

	bool bIsPaused = false; // 게임이 멈춤 상태인지

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	
	UPROPERTY()
	UHUDWidget *HUDWidget;

	UPROPERTY()
    APauseMenuManager* PauseMenuManager; // 일시 정지 메뉴 전체를 관리하는 매니저

    UPROPERTY(EditAnywhere)
    TSubclassOf<APauseMenuManager> PauseMenuManagerClass;


	UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> PauseMenuClass;

    UPROPERTY()
    UPauseMenuWidget* PauseMenuWidget;

	void LoadLevelAfterDelay();
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	void TogglePauseMenu();

	float GetBonusLootMultiplier() const;
	void SetBonusLootMultiplier(float Multiplier);

	UHUDWidget* GetHUDWidget() const;
	APauseMenuManager* GetPauseMenuManager() const;
};
