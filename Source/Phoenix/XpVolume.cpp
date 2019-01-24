// Fill out your copyright notice in the Description page of Project Settings.

#include "XpVolume.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
AXpVolume::AXpVolume()
{
	OnActorBeginOverlap.AddDynamic(this, &AXpVolume::OnOverlap);

	SM_XpVolume = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_XpVolume"));
	RootComponent = SM_XpVolume;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(SM_XpVolume);
}

void AXpVolume::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		Player = Cast<APhoenixCharacter>(OtherActor);

		if (Player)
		{
			Player->EarnXp(100.0f);
		}
	}	

	Destroy();
}
