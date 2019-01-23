// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UPrimitiveComponent;
class USkeletalMeshComponent;
class UShapeComponent;

UCLASS()
class PHOENIX_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* WieldableMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* PickupTrigger;

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* PickupThumbnail;

	UFUNCTION()
		virtual void OnRadiusEnter(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	bool bIsActive;

	virtual void Hide(bool bVisible);
};
