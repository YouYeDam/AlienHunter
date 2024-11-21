// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHUNTER_API AGameMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // 게임 메뉴 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> GameMenuClass;

    // 미션 메뉴 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MissionMenuClass;

    // 상점 메뉴 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> ShopMenuClass;

    // 인벤토리 메뉴 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> InventoryMenuClass;

    // 캐릭터 메뉴 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> CharacterMenuClass;

    // 현재 활성화된 위젯
    UPROPERTY()
    class UUserWidget* CurrentWidget;

protected:
    virtual void BeginPlay() override;

public:
    // UI 전환 함수
    void ShowGameMenu();
    void ShowMissionMenu();
    void ShowShopMenu();
    void ShowInventoryMenu();
    void ShowCharacterMenu();
};
