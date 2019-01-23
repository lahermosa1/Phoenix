// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageVolume.h"
#include <Components/BoxComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <TimerManager.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/Engine.h>

// Sets default values
ADamageVolume::ADamageVolume()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = TriggerVolume;

	VisualFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VisualFX"));
	VisualFX->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	VisualFX->SetupAttachment(RootComponent);

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ADamageVolume::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ADamageVolume::OnOverlapEnd);

	bCanApplyDamage = false;
}

void ADamageVolume::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		bCanApplyDamage = true;
		Player = Cast<AActor>(OtherActor);
		HitResult = SweepResult;
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ADamageVolume::ApplyGenericDamage, 2.2f, true, 0.0f);
		
		// DEBUG
		FString msg = (TEXT("DAMAGE DELIVERED"));

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, msg);
	}
}

void ADamageVolume::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}

void ADamageVolume::ApplyGenericDamage()
{
	if (bCanApplyDamage)
	{
		UGameplayStatics::ApplyPointDamage(Player, 200.0f, GetActorLocation(), HitResult, nullptr, this, GenericDamageType);
	}
}
