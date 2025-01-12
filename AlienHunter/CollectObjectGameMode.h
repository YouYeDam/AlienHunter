// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienHunterGameMode.h"
#include "CollectObjectGameMode.generated.h"

UCLASS()
class ALIENHUNTER_API ACollectObjectGameMode : public AAlienHunterGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ItemCollected();

	int32 GetCurrentCollectingCount() const;
	int32 GetRequiredCollectingCount() const;

private:
	int32 CurrentCollectingCount = 0;
	int32 RequiredCollectingCount = 0;
};
