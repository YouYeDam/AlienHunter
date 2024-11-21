// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "EnemyKillGameMode.h"

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (HUD != nullptr) {
        HUD->AddToViewport();
    }
}

void AMainPlayerController::LoadLevelAfterDelay()
{
    UGameplayStatics::OpenLevel(this, FName("GameMenu"));
}

void AMainPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromParent();
    
    if (bIsWinner) {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr) {
            WinScreen->AddToViewport();
        }
    }
    else {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr) {
            LoseScreen->AddToViewport();
        }
    }

    AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PlayerCharacter)
    {
        int32 Energy = PlayerCharacter->GetGainedEnergy();
        int32 EXP = PlayerCharacter->GetGainedEXP();

        GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

        if (GameManager)
        {
            int32 CurrentEnergy = GameManager->GetEnergy();
            int32 CurrentEXP = GameManager->GetEXP();

            GameManager->SetEnergy(CurrentEnergy + Energy);
            GameManager->SetEXP(CurrentEXP + EXP);
            GameManager->GainMissionReward();

        }
    }

    GetWorldTimerManager().SetTimer(GameEndTimer, this, &AMainPlayerController::LoadLevelAfterDelay, GameEndDelay, false);
}
