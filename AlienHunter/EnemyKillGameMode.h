

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
	int32 GetDeadEnemyCount() const;
	int32 GetTotalEnemyCount() const;

private:
	int32 TotalEnemyCount = 0;
	int32 DeadEnemyCount = 0;
};
