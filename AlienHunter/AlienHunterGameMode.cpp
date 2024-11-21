// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienHunterGameMode.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void AAlienHunterGameMode::PawnKilled(APawn* PawnKilled)
{
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        return;
    }

    AMainCharacter* KilledCharacter = Cast<AMainCharacter>(PawnKilled);
    if (KilledCharacter)
    {
        int32 Energy = KilledCharacter->GetEnergy();
        int32 EXP = KilledCharacter->GetEXP();
        
        AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if (PlayerCharacter)
        {
            int32 PlayerEnergy = PlayerCharacter->GetGainedEnergy();
            int32 PlayerEXP = PlayerCharacter->GetGainedEXP();

            PlayerCharacter->SetGainedEnergy(PlayerEnergy + Energy);
            PlayerCharacter->SetGainedEXP(PlayerEXP + EXP);

            GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
            
            if (GameManager) 
            {
                int32 CurrentKillEnemyCount = GameManager->GetKillEnemyCount();
                GameManager->SetKillEnemyCount(CurrentKillEnemyCount + 1);
            }
        }
    }
}
