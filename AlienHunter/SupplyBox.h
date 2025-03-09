

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "SupplyBox.generated.h"


UCLASS()
class ALIENHUNTER_API ASupplyBox : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void Collect(AActor* Collector) override;

private:
	int32 HealKitCount = 0;
	float AmmoValue = 0;
	int32 GrenadeCount = 0;
	void CalculatRandomValue();
};
