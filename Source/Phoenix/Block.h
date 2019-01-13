// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>
#include "Block.generated.h"

UCLASS()
class PHOENIX_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* SM_Block;

	uint8 MinimumMaterial;

	UPROPERTY(EditDefaultsOnly)
		float Resistance;

	UPROPERTY(BlueprintReadWrite)
		float BreakingStage;

	// Called when player wants to break block down to a further stage

	void Break();

	void ResetBlock();

	// Called when block hits the final breaking stage
	void OnBroken(bool HasRequiredPickaxe);

};
