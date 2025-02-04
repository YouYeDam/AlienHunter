

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMenuGameMode.generated.h"


UCLASS()
class ALIENHUNTER_API AGameMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> GameMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MissionMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> ShopMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> InventoryMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> CharacterMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PerkMenuClass;

    // 현재 활성화된 위젯
    UPROPERTY()
    class UUserWidget* CurrentWidget;

protected:
    virtual void BeginPlay() override;

public:
    void ShowGameMenu();

    void ShowMissionMenu();

    void ShowShopMenu();

    void ShowInventoryMenu();
    
    void ShowCharacterMenu();

    void ShowPerkMenu();
};
