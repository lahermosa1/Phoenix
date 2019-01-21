// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"
#include <Kismet/GameplayStatics.h>
#include "Phoenix.h"
#include "PhoenixCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WieldableMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("WieldableMesh"));

	PickupTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetGenerateOverlapEvents(true);
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnRadiusEnter);
	PickupTrigger->SetupAttachment(WieldableMesh);

	bIsActive = true;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotates ItemBase every frame
	FRotator rotation = WieldableMesh->GetComponentRotation();
	rotation.Yaw += 1.0f;
	WieldableMesh->SetRelativeRotation(rotation);
}

void AItemBase::OnRadiusEnter(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bIsActive)
		{
		APhoenixCharacter * Character = Cast<APhoenixCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		Character->AddItemToInventory(this);
		Hide(true);
		}
}

void AItemBase::Hide(bool bVisible)
{
	WieldableMesh->SetVisibility(false);
	bIsActive = !bVisible;
}

void AItemBase::OnPickedUp()
{

}

void AItemBase::OnUsed()
{
	Destroy();
}
