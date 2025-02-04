// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameManager.h"
#include "HUDWidget.h"
#include "MainPlayerController.generated.h"

UCLASS()
class ALIENHUNTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UGameManager* GameManager;

	UPROPERTY(EditAnywhere)
	float GameEndDelay = 5; // 게임 종료 후 레벨 로드 딜레이 시간

	FTimerHandle GameEndTimer; // 게임 종료 후 레벨 로드 타이머

	float BonusLootMultiplier = 1.0f; // 보너스 보상 배율

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHUDWidget> HUDClass;
	
	UPROPERTY(EditAnywhere)
	UHUDWidget *HUDWidget;

	void LoadLevelAfterDelay();
	
public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	float GetBonusLootMultiplier() const;
	void SetBonusLootMultiplier(float Multiplier);

	UHUDWidget* GetHUDWidget() const;
};
