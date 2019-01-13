// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Reference: https://www.youtube.com/watch?v=9IY_QzolwkE&list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA&index=6


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PhoenixGameMode.generated.h"

UCLASS(minimalapi)
class APhoenixGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

public:
	APhoenixGameMode();

	enum EHUDState : uint8
	{
		HS_Ingame, // 0
		HS_Inventory, // 1
		HS_Craft_Menu // 2
	};

	// Checks HUD state and then calls ApplyHUD
	void ApplyHUDChanges();

	// Gets the current HUD State
	uint8 GetHUDState();

	// Setter function for HUD state: applies the new state then calls ApplyHUDChanges
	UFUNCTION(BlueprintCallable, Category = "C++ UFunctions")
	void ChangeHUDState(uint8 NewState);

	// Applies HUD to the screen, returns true if successful, else false
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents);


protected:

	// current HUDState
	uint8 HUDState;

	// In game HUD 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ UProperties", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> IngameHUDClass;

	// Inventory HUD 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ UProperties", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InventoryHUDClass;

	// Crafting Menu HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ UProperties", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> CraftingMenuHUDClass;

	// Current HUD being displayed on the screen
	class UUserWidget* CurrentHUDWidget;
};



