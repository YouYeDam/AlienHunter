// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManager.h"
#include "AlienHunterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHUNTER_API AAlienHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PawnKilled(APawn* PawnKilled);

private:
	UPROPERTY()
	UGameManager* GameManager;
};
