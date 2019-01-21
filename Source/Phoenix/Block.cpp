// Fill out your copyright notice in the Description page of Project Settings.
// Reference (Spawn with key press): https://www.youtube.com/watch?v=Z-mbe4k5Wa4
// Reference (MC C++): https://www.youtube.com/playlist?list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA

#include "Block.h"
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Engine/Engine.h>
#include "ItemBlock.h"

// Sets default values
ABlock::ABlock()
{
	SM_Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));

	Resistance = 20.0f;  // by default
	BreakingStage = 0.0f;  // by default
	MinimumMaterial = 0; // for now

	FP_BlockDropZone = CreateDefaultSubobject<USceneComponent>(TEXT("BlockDropZone"));
	FP_BlockDropZone->SetupAttachment(SM_Block);
	FP_BlockDropZone->SetRelativeLocation(FVector(0.2f, 48.4f, 10.0f));

	bIsActive = true;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}


// =======================================
//		BLOCK BREAKING LOGIC
// =======================================
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

void ABlock::SpawnItemBlock()
{
	if (ToSpawn) 
	{
		UWorld* world = GetWorld();
		
		if (world) 
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			FRotator rotator;
			
			FVector spawnLocation = this->SM_Block->GetComponentLocation();
			
			world->SpawnActor<AItemBlock>(ToSpawn, spawnLocation, rotator, spawnParams);

			// DEBUG
			FString msg = (TEXT("ItemBlock spawned"));

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, msg);
		}
	}
}

void ABlock::OnBroken(bool HasRequiredPickaxe)
{	
	Destroy();
}

void ABlock::Hide(bool bVisible)
{
	SM_Block->SetVisibility(false);

	bIsActive = !bVisible;
}
