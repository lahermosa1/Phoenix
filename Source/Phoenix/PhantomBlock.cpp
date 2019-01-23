// Fill out your copyright notice in the Description page of Project Settings.

#include "PhantomBlock.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ShapeComponent.h>
#include <Components/BoxComponent.h>
#include <Engine/Engine.h>

// Sets default values
APhantomBlock::APhantomBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_PhantomBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PhantomBlockMesh"));

	DestroyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DestroyTrigger"));
	DestroyTrigger->SetGenerateOverlapEvents(true);
	DestroyTrigger->OnComponentBeginOverlap.AddDynamic(this, &APhantomBlock::OnRadiusEnter);
	DestroyTrigger->SetupAttachment(SM_PhantomBlock);
}

// Called when the game starts or when spawned
void APhantomBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhantomBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotates every frame
	FRotator rotation = SM_PhantomBlock->GetComponentRotation();
	rotation.Yaw += 1.0f;
	SM_PhantomBlock->SetRelativeRotation(rotation);

}

void APhantomBlock::OnRadiusEnter(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Destroy();

	// DEBUG
	FString msg = (TEXT("PhantomBlock DESTROYED"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, msg);
}

