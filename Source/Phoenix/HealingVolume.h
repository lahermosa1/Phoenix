// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhoenixCharacter.h"
#include "HealingVolume.generated.h"


class USphereComponent;

UCLASS()
class PHOENIX_API AHealingVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingVolume();

public:

	UFUNCTION()
		void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		APhoenixCharacter* Player;

	UPROPERTY(EditAnywhere)
		USphereComponent* TriggerVolume;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* SM_HealingVolume;
};
