// 미션 시 기본 에일리언 헌터 게임 모드

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlienHunterGameMode.generated.h"

class UGameManager;
class UHUDWidget;

UCLASS()
class ALIENHUNTER_API AAlienHunterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

	virtual void PawnKilled(APawn* PawnKilled);

	void UpdateHUDReference();
	
protected:
	virtual void EndGame(bool bIsPlayerWinner);
	
    void UpdateHUDMissionProgress();

private:
	UPROPERTY()
	UGameManager* GameManager;

	UPROPERTY()
    UHUDWidget* HUDWidget;
};
