// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.h"
#include "CharacterMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHUNTER_API UCharacterMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void OnMoveToGameMenuClicked();

	void ShowPlayerStat();
    
protected:
    virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UGameManager* GameManager;

    UPROPERTY(meta = (BindWidget))
    class UButton* GameMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LevelText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HealthText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnergyText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EXPText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KillEnemyCountText;

};
