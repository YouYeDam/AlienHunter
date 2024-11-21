// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyKillGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "MeleeAIController.h"

void AEnemyKillGameMode::BeginPlay()
{
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) 
    {
        AllEnemyCount++;
    }

    for (AMeleeAIController* Controller : TActorRange<AMeleeAIController>(GetWorld())) 
    {
        AllEnemyCount++;
    }
}

void AEnemyKillGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr) 
    {
        PlayerController->GameHasEnded(nullptr, false);
    }

    AShooterAIController* ShooterController = Cast<AShooterAIController>(PawnKilled->GetController());
    AMeleeAIController* MeleeController = Cast<AMeleeAIController>(PawnKilled->GetController());

    if (ShooterController != nullptr || MeleeController != nullptr) 
    {
        DeadEnemyCount++; 
    }

    if (DeadEnemyCount >= AllEnemyCount) 
    {
        EndGame(true);
    }
}

void AEnemyKillGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld())) {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}


