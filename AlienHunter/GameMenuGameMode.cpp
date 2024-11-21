// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuGameMode.h"
#include "ShopMenuWidget.h"
#include "CharacterMenuWidget.h"
#include "Blueprint/UserWidget.h"

void AGameMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }

    // 초기 메뉴 표시
    ShowGameMenu();
}

void AGameMenuGameMode::ShowGameMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (GameMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AGameMenuGameMode::ShowMissionMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (MissionMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MissionMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AGameMenuGameMode::ShowShopMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (ShopMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ShopMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UShopMenuWidget* ShopMenuWidget = Cast<UShopMenuWidget>(CurrentWidget);
            if (ShopMenuWidget)
            {
                ShopMenuWidget->ShowCurrentEnergy();
            }
        }
    }
}

void AGameMenuGameMode::ShowInventoryMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (InventoryMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AGameMenuGameMode::ShowCharacterMenu()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (CharacterMenuClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), CharacterMenuClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            UCharacterMenuWidget* CharacterMenuWidget = Cast<UCharacterMenuWidget>(CurrentWidget);
            if (CharacterMenuWidget)
            {
                CharacterMenuWidget->ShowPlayerStat();
            }
        }
    }
}

