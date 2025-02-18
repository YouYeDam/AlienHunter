

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundClass.h" 
#include "Sound/SoundMix.h"
#include "GameManager.h"
#include "SoundMenuWidget.generated.h"

UCLASS()
class ALIENHUNTER_API USoundMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnMoveToReturnToMenuMenuClicked();

	UFUNCTION()
	void OnMasterVolumeSliderValueChanged(float Value);

	UFUNCTION()
	void OnMasterVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnSFXVolumeSliderValueChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnBGMVolumeSliderValueChanged(float Value);

	UFUNCTION()
	void OnBGMVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnUIVolumeSliderValueChanged(float Value);

	UFUNCTION()
	void OnUIVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void InitializeVolumeSettings();

	UFUNCTION()
	void SetIsIngame(bool bInIsInGame);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMenuButton;

	UPROPERTY(meta = (BindWidget))
	class USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    class USpinBox* MasterVolumeSpinBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* SFXVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    class USpinBox* SFXVolumeSpinBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* BGMVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    class USpinBox* BGMVolumeSpinBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* UIVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    class USpinBox* UIVolumeSpinBox;

	UPROPERTY(EditAnywhere)
	USoundClass* MasterSoundClass;

	UPROPERTY(EditAnywhere)
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere)
	USoundClass* BGMSoundClass;

	UPROPERTY(EditAnywhere)
	USoundClass* UISoundClass;

	UPROPERTY(EditAnywhere)
	USoundMix* MasterSoundMix;

	UPROPERTY()
	UGameManager* GameManager;

	UPROPERTY()
	bool bIsInGame = false;

	// 볼륨 값
	float MasterVolume = 1.0f;
	float SFXVolume = 1.0f;
	float BGMVolume = 1.0f;
	float UIVolume = 1.0f;

	void ApplyVolumeSettings();
};
