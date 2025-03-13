


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/CrowdFollowingComponent.h" 
#include "BTTask_Roam.h"
#include "BTTask_CombatLocationWait.h"
#include "BaseAIController.generated.h"

UCLASS()
class ALIENHUNTER_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	bool bIsInCombat = false;

public:
	ABaseAIController();

	virtual void Tick(float DeltaTime) override;
	
	void OnMonsterDamaged();

	void SetInCombat(bool Value);

	void StartRoaming(UBTTask_Roam* Task);

	void StartCheckingCombatLocationWait(UBTTask_CombatLocationWait* Task);

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	bool IsDead() const;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

    UPROPERTY()
    UBTTask_Roam* CurrentTask_Roam;

	UPROPERTY()
	UBTTask_CombatLocationWait* CurrentTask_CombatLocationWait;
};
