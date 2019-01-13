// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Reference: https://www.youtube.com/watch?v=9IY_QzolwkE&list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA&index=6


#include "PhoenixGameMode.h"
#include "PhoenixHUD.h"
#include "PhoenixCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>

void APhoenixGameMode::BeginPlay()
{
	Super::BeginPlay();

	ApplyHUDChanges();
}

APhoenixGameMode::APhoenixGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APhoenixHUD::StaticClass();

	HUDState = EHUDState::HS_Ingame;
}

void APhoenixGameMode::ApplyHUDChanges()
{
	// Remove previous HUD before applying new HUD
	if(CurrentHUDWidget != nullptr)
	{
		CurrentHUDWidget->RemoveFromParent();
	}

	// Check HUD state and apply appropriate HUD
	switch(HUDState)
	{
		case EHUDState::HS_Ingame:
		{
			ApplyHUD(IngameHUDClass, false, false); // false for bShowMouseCursor, false for bEnableClickEvents
		}

		case EHUDState::HS_Inventory:
		{
			ApplyHUD(InventoryHUDClass, true, true); // false for bShowMouseCursor, false for bEnableClickEvents
		}

		case EHUDState::HS_Craft_Menu:
		{
			ApplyHUD(CraftingMenuHUDClass, true, true); // false for bShowMouseCursor, false for bEnableClickEvents
		}
		default:
		{
			ApplyHUD(IngameHUDClass, false, false);
		}
	
	}

}

uint8 APhoenixGameMode::GetHUDState()
{
	return HUDState;
}

void APhoenixGameMode::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool APhoenixGameMode::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool ShowMouseCursor, bool EnableClickEvents)
{
	//get player reference & player controller
	APhoenixCharacter* MyCharacter = Cast<APhoenixCharacter>(UGameplayStatics::GetPlayerController(this, 0));
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	//null check widget b4 applying it (to prevent game from crashing)
	if (WidgetToApply != nullptr)
	{
		// set mouse events and visibility parameters taken by the function
		MyController->bShowMouseCursor = ShowMouseCursor;
		MyController->bEnableClickEvents = EnableClickEvents;

		// Create the widget
		CurrentHUDWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		// null check current widget

		if(CurrentHUDWidget != nullptr)
		{
			CurrentHUDWidget->AddToViewport();
			return true;
		}
		else return false;
	}
	else return false;
}
