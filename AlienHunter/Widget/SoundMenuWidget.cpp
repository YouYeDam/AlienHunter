


#include "SoundMenuWidget.h"
#include "MainPlayerController.h"
#include "PauseMenuManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/SpinBox.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "GameMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"

void USoundMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameManager = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (ReturnToMenuButton)
    {
        ReturnToMenuButton->OnClicked.AddDynamic(this, &USoundMenuWidget::OnMoveToReturnToMenuMenuClicked);
    }

    if (MasterSoundMix)
    {
        UGameplayStatics::SetBaseSoundMix(GetWorld(), MasterSoundMix);
    }

    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USoundMenuWidget::OnMasterVolumeSliderValueChanged);
    }

    if (MasterVolumeSpinBox)
    {
        MasterVolumeSpinBox->OnValueCommitted.AddDynamic(this, &USoundMenuWidget::OnMasterVolumeTextCommitted);
    }

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USoundMenuWidget::OnSFXVolumeSliderValueChanged);
    }

    if (SFXVolumeSpinBox)
    {
        SFXVolumeSpinBox->OnValueCommitted.AddDynamic(this, &USoundMenuWidget::OnSFXVolumeTextCommitted);
    }

    if (BGMVolumeSlider)
    {
        BGMVolumeSlider->OnValueChanged.AddDynamic(this, &USoundMenuWidget::OnBGMVolumeSliderValueChanged);
    }

    if (BGMVolumeSpinBox)
    {
        BGMVolumeSpinBox->OnValueCommitted.AddDynamic(this, &USoundMenuWidget::OnBGMVolumeTextCommitted);
    }

    if (UIVolumeSlider)
    {
        UIVolumeSlider->OnValueChanged.AddDynamic(this, &USoundMenuWidget::OnUIVolumeSliderValueChanged);
    }

    if (UIVolumeSpinBox)
    {
        UIVolumeSpinBox->OnValueCommitted.AddDynamic(this, &USoundMenuWidget::OnUIVolumeTextCommitted);
    }
    
    InitializeVolumeSettings();
}

// 일시정지 메뉴 혹은 게임 메뉴로 돌아가는 메소드
void USoundMenuWidget::OnMoveToReturnToMenuMenuClicked()
{
    if (bIsInGame) // 미션 중
    {
        AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());

        if (PlayerController)
        {
            APauseMenuManager* PauseMenuManager = PlayerController->GetPauseMenuManager();
            if (PauseMenuManager)
            {
                PauseMenuManager->ShowPauseMenu();
            }
        }
    }
    else
    {
        AGameMenuGameMode* GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode)
        {
            GameMode->ShowGameMenu();
        }
    }
}

// 슬라이더를 통해 마스터 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnMasterVolumeSliderValueChanged(float Value)
{
    MasterVolume = Value;

    if (MasterVolumeSpinBox)
    {
        MasterVolumeSpinBox->SetValue(MasterVolume);
    }

    if (GameManager)
    {
        GameManager->SetMasterVolume(MasterVolume);
    }

    ApplyVolumeSettings();
}

// 텍스트 박스를 통해 마스터 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnMasterVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod)
{
    MasterVolume = FMath::Clamp(Value, 0.0f, 1.0f);

    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(MasterVolume);
    }

    if (GameManager)
    {
        GameManager->SetMasterVolume(MasterVolume);
    }

    ApplyVolumeSettings();
}

// 슬라이더를 통해 SFX 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnSFXVolumeSliderValueChanged(float Value)
{
    SFXVolume = Value;

    if (SFXVolumeSpinBox)
    {
        SFXVolumeSpinBox->SetValue(SFXVolume);
    }

    if (GameManager)
    {
        GameManager->SetSFXVolume(SFXVolume);
    }
    
    ApplyVolumeSettings();
}

// 텍스트 박스를 통해 SFX 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnSFXVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod)
{
    SFXVolume = FMath::Clamp(Value, 0.0f, 1.0f);

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(SFXVolume);
    }

    if (GameManager)
    {
        GameManager->SetSFXVolume(SFXVolume);
    }

    ApplyVolumeSettings();
}

// 슬라이더를 통해 BGM 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnBGMVolumeSliderValueChanged(float Value)
{
    BGMVolume = Value;

    if (BGMVolumeSpinBox)
    {
        BGMVolumeSpinBox->SetValue(BGMVolume);
    }

    if (GameManager)
    {
        GameManager->SetBGMVolume(BGMVolume);
    }

    ApplyVolumeSettings();
}

// 텍스트 박스를 통해 BGM 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnBGMVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod)
{
    BGMVolume = FMath::Clamp(Value, 0.0f, 1.0f);

    if (BGMVolumeSlider)
    {
        BGMVolumeSlider->SetValue(BGMVolume);
    }

    if (GameManager)
    {
        GameManager->SetBGMVolume(BGMVolume);
    }

    ApplyVolumeSettings();
}

// 슬라이더를 통해 UI 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnUIVolumeSliderValueChanged(float Value)
{
    UIVolume = Value;

    if (UIVolumeSpinBox)
    {
        UIVolumeSpinBox->SetValue(UIVolume);
    }

    if (GameManager)
    {
        GameManager->SetUIVolume(UIVolume);
    }

    ApplyVolumeSettings();
}

// 텍스트 박스를 통해 UI 볼륨 값을 변경하는 메소드
void USoundMenuWidget::OnUIVolumeTextCommitted(float Value, ETextCommit::Type CommitMethod)
{
    UIVolume = FMath::Clamp(Value, 0.0f, 1.0f);

    if (UIVolumeSlider)
    {
        UIVolumeSlider->SetValue(UIVolume);
    }

    if (GameManager)
    {
        GameManager->SetUIVolume(UIVolume);
    }
    
    ApplyVolumeSettings();
}

// 볼륨 슬라이더 초기값을 설정하는 메소드
void USoundMenuWidget::InitializeVolumeSettings()
{
    if (GameManager)
    {
        MasterVolume = GameManager->GetMasterVolume();
        SFXVolume = GameManager->GetSFXVolume();
        BGMVolume = GameManager->GetBGMVolume();
        UIVolume = GameManager->GetUIVolume();
    }
    
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(MasterVolume);
    }
    if (MasterVolumeSpinBox)
    {
        MasterVolumeSpinBox->SetValue(MasterVolume);
    }

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(SFXVolume);
    }
    if (SFXVolumeSpinBox)
    {
        SFXVolumeSpinBox->SetValue(SFXVolume);
    }

    if (BGMVolumeSlider)
    {
        BGMVolumeSlider->SetValue(BGMVolume);
    }
    if (BGMVolumeSpinBox)
    {
        BGMVolumeSpinBox->SetValue(BGMVolume);
    }

    if (UIVolumeSlider)
    {
        UIVolumeSlider->SetValue(UIVolume);
    }
    if (UIVolumeSpinBox)
    {
        UIVolumeSpinBox->SetValue(UIVolume);
    }

    ApplyVolumeSettings();
}

// 볼륨을 적용하는 메소드
void USoundMenuWidget::ApplyVolumeSettings()
{
    if (MasterSoundClass)
    {
        MasterSoundClass->Properties.Volume = MasterVolume;
    }

    if (SFXSoundClass)
    {
        SFXSoundClass->Properties.Volume = MasterVolume * SFXVolume;
    }

    if (BGMSoundClass)
    {
        BGMSoundClass->Properties.Volume = MasterVolume * BGMVolume;
    }

    if (UISoundClass)
    {
        UISoundClass->Properties.Volume = MasterVolume * UIVolume;
    }
}

void USoundMenuWidget::SetIsIngame(bool bInIsInGame)
{
    bIsInGame = bInIsInGame;
}