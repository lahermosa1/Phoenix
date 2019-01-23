// Fill out your copyright notice in the Description page of Project Settings.
// Reference (MC C++): https://www.youtube.com/playlist?list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SceneComponent.h>
#include "Block.generated.h"


UCLASS()
class PHOENIX_API ABlock : public AActor
{
	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ Variables")
		class USceneComponent* FP_BlockDropZone;

	
public:	
	// Sets default values for this actor's properties
	ABlock();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* SM_Block;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Variables")
		UTexture2D* PickupThumbnail;

	uint8 MinimumMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "C++ Variables")
		float Resistance;

	UPROPERTY(BlueprintReadWrite, Category = "C++ Variables")
		float BreakingStage;


	// Called when player wants to break block down to a further stage

	void Break();

	void ResetBlock();

	UPROPERTY(EditDefaultsOnly, Category = "C++ Variables")
		TSubclassOf<class AItemBlock> SpawnedItemBlock;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void SpawnItemBlock();

// 	UPROPERTY(EditDefaultsOnly, Category = "C++ Variables")
// 		TSubclassOf<class APhantomBlock> SpawnedPhantomBlock;
// 
// 	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
// 		void SpawnPhantomBlock();

	// Called when block hits the final breaking stage
	void OnBroken(bool HasRequiredPickaxe);

	bool bIsActive;

	void Hide(bool bVisible);


// =======================================================================
// Spawning smaller block: https://www.youtube.com/watch?v=Z-mbe4k5Wa4
// =======================================================================

};
