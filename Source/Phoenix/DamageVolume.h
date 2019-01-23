// Fill out your copyright notice in the Description page of Project Settings.
// Reference: https://unrealcpp.com/health-bar-ui-hud/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageVolume.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class PHOENIX_API ADamageVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageVolume();

public:

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* VisualFX;

	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> GenericDamageType;

	UPROPERTY(EditAnywhere)
		AActor* Player;

	UPROPERTY(EditAnywhere)
		FHitResult HitResult;

	bool bCanApplyDamage;
	FTimerHandle DamageTimerHandle;

	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ApplyGenericDamage();

};
