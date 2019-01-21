// Fill out your copyright notice in the Description page of Project Settings.
// Reference: https://answers.unrealengine.com/questions/698070/error-c2664-cannot-convert-argument-2-from-void-cd.html

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.h"
#include "Wieldable.generated.h"

UCLASS()
class PHOENIX_API AWieldable : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWieldable();

public:	

	enum ETool : uint8
	{
		Unarmed,
		Hoe,
		Axe,
		Pickaxe,
		Shovel,
		Sword,
		Battleaxe,
		Dagger,
		Bow
	};

	enum EMaterial : uint8
	{
		// Number determines how fast blocks will break
		None = 1,
		Wooden = 2,
		Copper = 3,
		Stone = 4,
		Iron = 6,
		Golden = 10,
		Silver = 12,
		Serpentinite = 15,
		Phoenixstone = 20
	};

	UPROPERTY(EditAnywhere)
	uint8 ToolType;

	UPROPERTY(EditAnywhere)
	uint8 MaterialType;
};
