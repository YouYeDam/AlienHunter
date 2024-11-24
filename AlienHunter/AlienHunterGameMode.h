// 미션 시 기본 에일리언 헌터 게임 모드

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManager.h"
#include "AlienHunterGameMode.generated.h"


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
