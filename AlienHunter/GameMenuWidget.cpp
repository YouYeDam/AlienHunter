// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameMode = Cast<AGameMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (MissionMenuButton)
    {
        MissionMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToMissionMenuClicked);
    }

    if (ShopMenuButton)
    {
        ShopMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToShopMenuClicked);
    }

    if (InventoryMenuButton)
    {
        InventoryMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToInventoryMenuClicked);
    }

    if (CharacterMenuButton)
    {
        CharacterMenuButton->OnClicked.AddDynamic(this, &UGameMenuWidget::OnMoveToCharacterMenuClicked);
    }
}

void UGameMenuWidget::OnMoveToMissionMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowMissionMenu();
    }
}

void UGameMenuWidget::OnMoveToShopMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowShopMenu();
    }
}

void UGameMenuWidget::OnMoveToInventoryMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowInventoryMenu();
    }
}

void UGameMenuWidget::OnMoveToCharacterMenuClicked()
{
    if (GameMode)
    {
        GameMode->ShowCharacterMenu();
    }
}


