// Fill out your copyright notice in the Description page of Project Settings.
// Reference (Spawn with key press): https://www.youtube.com/watch?v=Z-mbe4k5Wa4
// Reference (MC C++): https://www.youtube.com/playlist?list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA

#include "Block.h"
#include <Components/StaticMeshComponent.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Engine/Engine.h>
#include "ItemBlock.h"
#include "PhantomBlock.h"

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
	if (SpawnedItemBlock)
	{
		UWorld* world = GetWorld();
		
		if (world) 
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			FRotator rotator;
			
			FVector spawnLocation = SM_Block->GetComponentLocation() + FVector(0.2f, 48.4f, 10.0f);
			
			world->SpawnActor<AItemBlock>(SpawnedItemBlock, spawnLocation, rotator, spawnParams);
			

			// DEBUG
			FString msg = (TEXT("ItemBlock SPAWNED"));

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, msg);
		}
	}
}

// void ABlock::SpawnPhantomBlock()
// {
// 	if (SpawnedPhantomBlock)
// 	{
// 		UWorld* world = GetWorld();
// 
// 		if (world)
// 		{
// 			FActorSpawnParameters spawnParams;
// 			spawnParams.Owner = this;
// 			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
// 
// 			FRotator rotator;
// 
// 			FVector spawnLocation = SM_Block->GetComponentLocation() + FVector(0.2f, 48.4f, 10.0f);
// 
// 			world->SpawnActor<APhantomBlock>(SpawnedPhantomBlock, spawnLocation, rotator, spawnParams);
// 
// 
// 			// DEBUG
// 			FString msg = (TEXT("PhantomBlock SPAWNED"));
// 
// 			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, msg);
// 		}
// 	}
// }

/* Try OnBroken... shrink the broken block, make it float and rotate.
   Then the OnOverlap fxn can work on this smaller block */
void ABlock::OnBroken(bool HasRequiredPickaxe)
{	
	SpawnItemBlock();

	/*SpawnPhantomBlock();*/

	Destroy();
}

void ABlock::Hide(bool bVisible)
{
	SM_Block->SetVisibility(false);

	bIsActive = !bVisible;
}
