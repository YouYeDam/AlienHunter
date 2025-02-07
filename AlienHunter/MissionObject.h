

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "MissionObject.generated.h"

UCLASS()
class ALIENHUNTER_API AMissionObject : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void Collect(AActor* Collector) override;
};
