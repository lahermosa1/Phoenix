// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wieldable.generated.h"


class UPrimitiveComponent;
class USkeletalMeshComponent;
class UShapeComponent;

UCLASS()
class PHOENIX_API AWieldable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWieldable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WieldableMesh;

	UPROPERTY(EditAnywhere)
	UShapeComponent* PickupTrigger;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* PickupThumbnail;

	bool bIsActive;

	UFUNCTION()
	void OnRadiusEnter(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void OnPickedUp();

	void OnUsed();

	// void AWieldable::OnRadiusEnter(UPrimitiveComponent* OverlappedComponent, 
	//AActor OtherActor, UPrimitiveComponent OtherComp, int32 OtherBodyIndex, 
	// bool bFromSweep, const FHitResult & SweepResult)
	// Reference: https://answers.unrealengine.com/questions/698070/error-c2664-cannot-convert-argument-2-from-void-cd.html
};
