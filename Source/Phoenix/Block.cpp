// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include <Materials/MaterialInstanceDynamic.h>

// Sets default values
ABlock::ABlock()
{
	SM_Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));

	Resistance = 20.0f;  // by default
	BreakingStage = 0.0f;  // by default
	MinimumMaterial = 0; // for now
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlock::Break()
{
	++BreakingStage;

	float UnbrokenPercent = 1.0f - (BreakingStage / 5.0f);

	UMaterialInstanceDynamic* MatInstance = SM_Block->CreateDynamicMaterialInstance(0);

	if(MatInstance != nullptr)
	{
		MatInstance->SetScalarParameterValue(FName("UnbrokenPercent"), UnbrokenPercent);
	}

	if(BreakingStage == 5.0f)
	{
		OnBroken(true);
	}
}

void ABlock::ResetBlock()
{
	BreakingStage = 0.0f;

	UMaterialInstanceDynamic* MatInstance = SM_Block->CreateDynamicMaterialInstance(0);

	if(MatInstance != nullptr)
	{
		MatInstance->SetScalarParameterValue(FName("UnbrokenPercent"), 1.0f);
	}
}

void ABlock::OnBroken(bool HasRequiredPickaxe)
{
	Destroy();
}
