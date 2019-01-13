// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PhoenixCharacter.h"
#include "PhoenixProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include <Engine/Engine.h>
#include <GameFramework/Actor.h>

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APhoenixCharacter

APhoenixCharacter::APhoenixCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 75.0f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	//Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_WieldedItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_WieldedItem->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_WieldedItem->bCastDynamicShadow = false;
	FP_WieldedItem->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_WieldedItem->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_WieldedItem);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	Reach = 250.0f;

	Inventory.SetNum(NUM_OF_INVENTORY_SLOTS);

}

void APhoenixCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_WieldedItem->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Health = 1.0f;
	Food = 1.0f;
	Water = 1.0f;
	Stamina = 1.0f;
	Experience = 0.0f;
	Level = 1;
	CoinGold = 0;
	CoinSilver = 0;
	CoinCopper = 0;
}

void APhoenixCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForBlock();
}


int32 APhoenixCharacter::GetCurrentInventorySlot()
{
	return CurrentInventorySlot;
}

bool APhoenixCharacter::AddItemToInventory(AWieldable* Item)
{
	if(Item != NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);

		if(AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
		else return false;
	}

	return false;
}

UTexture2D* APhoenixCharacter::GetThumbnailAtInventorySlot(uint8 Slot)
{
	if(Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupThumbnail;
	}
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void APhoenixCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Inventory events
	PlayerInputComponent->BindAction("InventoryScrollUp", IE_Pressed, this, &APhoenixCharacter::MoveUpInventorySlot);
	PlayerInputComponent->BindAction("InventoryScrollDown", IE_Pressed, this, &APhoenixCharacter::MoveDownInventorySlot);


	// Bind OnHit event
	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &APhoenixCharacter::OnHit);
	PlayerInputComponent->BindAction("Hit", IE_Released, this, &APhoenixCharacter::EndHit);

	// Bind OnInteract event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APhoenixCharacter::OnInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APhoenixCharacter::EndInteract);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APhoenixCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APhoenixCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APhoenixCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APhoenixCharacter::LookUpAtRate);
}

void APhoenixCharacter::MoveUpInventorySlot()
{
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot + 1) % NUM_OF_INVENTORY_SLOTS);
}

void APhoenixCharacter::MoveDownInventorySlot()
{
	if(CurrentInventorySlot == 0)
	{
		CurrentInventorySlot = 9;
		return;
	}
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot - 1) % NUM_OF_INVENTORY_SLOTS);
}

// RMB
void APhoenixCharacter::OnInteract()
{
	// Debug
	FString interactmsg = (TEXT("Interacted using RMB"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, interactmsg);
}

void APhoenixCharacter::EndInteract()
{

}

void APhoenixCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APhoenixCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APhoenixCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APhoenixCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// LMB
void APhoenixCharacter::OnHit()
{
	PlayHitAnim();

	if (CurrentBlock != nullptr) 
	{

		bIsBreakingBlock = true;

		int ToolPower;
		ToolPower = 2; // Set this according to type of weapon/tool; Using 2 for now.
		float TimeBetweenHits = ((CurrentBlock->Resistance / 100.0f / ToolPower));

		GetWorld()->GetTimerManager().SetTimer(BlockBreakingHandle, this, &APhoenixCharacter::BreakBlock, TimeBetweenHits, true);
		GetWorld()->GetTimerManager().SetTimer(HitAnimHandle, this, &APhoenixCharacter::PlayHitAnim, 0.4f, true);

	}

	// Debug
	FString hitmsg = (TEXT("Hit using LMB"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, hitmsg);
}

void APhoenixCharacter::EndHit()
{
	GetWorld()->GetTimerManager().ClearTimer(BlockBreakingHandle);
	GetWorld()->GetTimerManager().ClearTimer(HitAnimHandle);

	bIsBreakingBlock = false;

	if (CurrentBlock != nullptr) 
	{
		CurrentBlock->ResetBlock();
	}
}

void APhoenixCharacter::PlayHitAnim()
{
	if (HitAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(HitAnimation, 1.f);
		}
	}
}

void APhoenixCharacter::CheckForBlock()
{
	FHitResult LinetraceHit;

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * Reach) + StartTrace;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(LinetraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CQP);

	ABlock* PotentialBlock = Cast<ABlock>(LinetraceHit.GetActor());
	
	// if player stops looking at block, reset the graphics
	if(PotentialBlock != CurrentBlock && CurrentBlock != nullptr)
	{
		CurrentBlock->ResetBlock();
	}

	if(PotentialBlock == NULL) 
	{
		CurrentBlock = nullptr;
		return;
	}
	else
	{
		if (CurrentBlock != nullptr && !bIsBreakingBlock) 
		{
			CurrentBlock->ResetBlock();
		}
		
		CurrentBlock = PotentialBlock;

		// Debug Msg
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *CurrentBlock->GetName());
	}
}

void APhoenixCharacter::BreakBlock()
{
	if (bIsBreakingBlock && CurrentBlock != nullptr && !CurrentBlock->IsPendingKill()) 
	{
		CurrentBlock->Break();
	}
}
