

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "MonsterCharacter.h"
#include "BTTask_CombatLocationWait.generated.h"

UCLASS()
class ALIENHUNTER_API UBTTask_CombatLocationWait : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:
	UBTTask_CombatLocationWait();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnMonsterDamaged(); // 몬스터가 피격될 때 실행될 함수

	AMonsterCharacter* MonsterCharacter; // 감시할 몬스터
};
