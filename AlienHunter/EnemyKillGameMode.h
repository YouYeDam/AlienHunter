

#pragma once

#include "CoreMinimal.h"
#include "AlienHunterGameMode.h"
#include "EnemyKillGameMode.generated.h"


UCLASS()
class ALIENHUNTER_API AEnemyKillGameMode : public AAlienHunterGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void PawnKilled(APawn* PawnKilled) override;	

private:
	int32 AllEnemyCount = 0;
	int32 DeadEnemyCount = 0;
	
	void EndGame(bool bIsPlayerWinner);
};
