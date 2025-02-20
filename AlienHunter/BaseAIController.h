


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/CrowdFollowingComponent.h" 
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
	
	void SetInCombat(bool Value);

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	bool IsDead() const;
};
