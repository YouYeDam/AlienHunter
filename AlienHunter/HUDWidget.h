#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

UCLASS()
class ALIENHUNTER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateMissionProgressText();
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
    UPROPERTY()
    class APlayerCharacter* PlayerCharacter;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* GainedEnergy;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* GainedEXP;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* MissionProgress;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Ammo;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* HealKit;

	void UpdateGainedEnergyText();
	void UpdateGainedEXPText();
	void UpdateAmmoText();
	void UpdateHealKitText();
};
