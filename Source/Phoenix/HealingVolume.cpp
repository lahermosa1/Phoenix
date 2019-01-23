// Fill out your copyright notice in the Description page of Project Settings.

#include "HealingVolume.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
AHealingVolume::AHealingVolume()
{
	OnActorBeginOverlap.AddDynamic(this, &AHealingVolume::OnOverlap);

	SM_HealingVolume = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_HealingVolume"));
	RootComponent = SM_HealingVolume;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(SM_HealingVolume);
}

void AHealingVolume::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if ((OtherActor != nullptr) && (OtherActor != this)) 
	{
		Player = Cast<APhoenixCharacter>(OtherActor);

		if (Player && Player->GetHealthCurrent() < 1.0f) 
		{
			Player->UpdateHealthCurrent(100.0f);
			Destroy();
		}
	}
}
