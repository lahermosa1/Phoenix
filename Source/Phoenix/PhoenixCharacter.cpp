// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Reference: https://github.com/carlso70/UE4-Minecraft/tree/master/Source/Minecraft
// Reference (MC C++): https://www.youtube.com/playlist?list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA
// Reference: https://unrealcpp.com/health-bar-ui-hud/
// Reference (correct character .h and .cpp code): https://github.com/Harrison1/unrealcpp/tree/master/HealthBar

#include "PhoenixCharacter.h"
#include "PhoenixProjectile.h"
#include "Wieldable.h"
#include "ItemBase.h"
#include "ItemBlock.h"
#include "Block.h"

#include <Animation/AnimInstance.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/InputComponent.h>
#include <GameFramework/Actor.h>
#include <GameFramework/InputSettings.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include <Engine/Engine.h>

#include <cmath>
#include <random>
#include <stdlib.h>

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


APhoenixCharacter::APhoenixCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(5.0f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Create a CameraComponent	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->RelativeLocation = FVector(-39.56f, 1.75f, 64.0f); // Position the camera
	Camera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	SKM_Player = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM_Player"));
	SKM_Player->SetupAttachment(Camera);
	SKM_Player->SetOnlyOwnerSee(true);
	SKM_Player->bCastDynamicShadow = true;  // made true
	SKM_Player->CastShadow = true;			// made true
	SKM_Player->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	SKM_Player->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	SKM_Player->SetGenerateOverlapEvents(true);

	// Create a gun mesh component
	SKM_WieldedItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM_WieldedItem"));
	SKM_WieldedItem->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	SKM_WieldedItem->SetupAttachment(SKM_Player, TEXT("GripPoint"));
	SKM_WieldedItem->bCastDynamicShadow = true;  // made true
	SKM_WieldedItem->CastShadow = true;			 // made true


	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(SKM_WieldedItem);
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

	HealthMax = 1000.0f;
	HealthCurrent = HealthMax;
	HealthPercent = 1.0f;
	bCanBeDamaged = true;

	FoodMax = 1000.0f;
	FoodCurrent = FoodMax;
	FoodPercent = 1.0f;
	FoodLossAmt = 0.1f;  // Lower later for gameplay (0.01f maybe?)

	XpMax = 1000.0f;
	XpCurrent = 0.0f;
	XpPercent = 0.0f;

	LevelMax = 100;
	LevelCurrent = 1;
	LevelNew = LevelCurrent;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	SKM_WieldedItem->AttachToComponent(SKM_Player, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void APhoenixCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForBlock();

	LoseFood(FoodLossAmt);
}


// =============================================================================
//		HEALTH
//		Reference: https://unrealcpp.com/health-bar-ui-hud/
// =============================================================================
float APhoenixCharacter::GetHealthCurrent()
{
	return HealthPercent;
}

FText APhoenixCharacter::GetHealthCurrentIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercent * 100);
	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);
	return HPText;
}

void APhoenixCharacter::UpdateHealthCurrent(float HealthChange)
{
	HealthCurrent = FMath::Clamp(HealthCurrent += HealthChange, 0.0f, HealthMax);
	HealthPercent = HealthCurrent / HealthMax;
}

// ================================================================================
//		FOOD
// ================================================================================

float APhoenixCharacter::GetFoodCurrent()
{
	return FoodPercent;
}

FText APhoenixCharacter::GetFoodCurrentIntText()
{
	int32 Food = FMath::RoundHalfFromZero(FoodPercent * 100);
	FString FoodString = FString::FromInt(Food);
	FString FoodHUDString = FoodString + FString(TEXT("%"));
	FText FoodText = FText::FromString(FoodHUDString);

	return FoodText;
}

/*  Call this function from FoodClass and simply add in Food value of FOodClass as FoodGain */
/* OR...create FoodClass with FoodValue, cast to the AFoodClass, then set FOodValue = to FoodGain */
float APhoenixCharacter::EatFood(float FoodGain)
{
	if (FoodCurrent < 1000.0f)
	{
		FoodCurrent += FoodGain;
	}
	if (FoodCurrent >= 1000.0f)
	{
		FoodCurrent = 1000.0f;
	}

	FoodPercent = FoodCurrent / FoodMax;

	return FoodPercent;
}

float APhoenixCharacter::LoseFood(float FoodLoss)
{
	if(FoodCurrent > 0.0f)
	{
		FoodCurrent -= FoodLoss;
	}
	if(FoodCurrent <= 0.0f)
	{
		FoodCurrent = 0.0f;
		/* Will start to lose health */
	}

	FoodPercent = FoodCurrent / FoodMax;
	
	return FoodPercent;
}


// ================================================================================
//		XP & LEVELING
// ================================================================================
float APhoenixCharacter::GetXpCurrent()
{
	return XpPercent;
}

FText APhoenixCharacter::GetXpCurrentIntText()
{
	int32 Xp = FMath::RoundHalfFromZero(XpPercent * 100);
	FString XpString = FString::FromInt(Xp);
	FString XpHUDString = XpString + FString(TEXT("%"));
	FText XpText = FText::FromString(XpHUDString);

	return XpText;
}

float APhoenixCharacter::EarnXp(float XpGain)
{
	XpCurrent += XpGain;

	if (XpCurrent >= XpMax) 
	{
		XpCurrent = 0.0f;

		LevelUp();
	}

	XpPercent = XpCurrent / XpMax;

	return XpPercent;
}

int APhoenixCharacter::GetLevelCurrent()
{
	return LevelNew;
}

int APhoenixCharacter::LevelUp()
{
	if(LevelCurrent < LevelMax)
	{
		LevelCurrent += 1;
	}
	if(LevelCurrent >= LevelMax)
	{
		LevelCurrent = LevelMax;
	}

	LevelNew = LevelCurrent;

	return LevelNew;
}

FText APhoenixCharacter::GetLevelCurrentIntText()
{
	int32 level = LevelNew;
	FString levelString = FString::FromInt(level);;
	FText levelText = FText::FromString(levelString);

	return levelText;
}

// ================================================================================
//		DAMAGE
//      Reference: https://unrealcpp.com/health-bar-ui-hud/
// ================================================================================
bool APhoenixCharacter::PlayRedScreenFlash()
{
	if(RedScreenFlash)
	{
		RedScreenFlash = false;
		return true;
	}
	return false;
}

void APhoenixCharacter::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APhoenixCharacter::SetDamageState, 2.0f, false);
}

void APhoenixCharacter::SetDamageState()
{
	bCanBeDamaged = true;
}

float APhoenixCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	bCanBeDamaged = false;
	RedScreenFlash = true;
	UpdateHealthCurrent(-DamageAmount);
	DamageTimer();
	return DamageAmount;

	// DEBUG
	FString msg = (TEXT("DAMAGE TAKEN"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, msg);
}

// =============================================================================
//		SKILL GROWTH FUNCTIONS
//		Reference: http://www.cplusplus.com/reference/cstdlib/rand/
// =============================================================================
int APhoenixCharacter::SkillDiceRollGrowthFast()
{
	int result = rand() % 4 + 2;
	return result;
}

int APhoenixCharacter::SkillDiceRollGrowthMed()
{
	int result = rand() % 2 + 1;
	return result;
}

int APhoenixCharacter::SkillDiceRollGrowthSlow()
{
	int result = rand() % 2;
	return result;
}

// ================================================================================
//		INPUT
// ================================================================================
void APhoenixCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Throw event
	InputComponent->BindAction("Throw", IE_Pressed, this, &APhoenixCharacter::Throw);

	// Bind Inventory events
	PlayerInputComponent->BindAction("InventoryScrollUp", IE_Pressed, this, &APhoenixCharacter::MoveUpInventorySlot);
	PlayerInputComponent->BindAction("InventoryScrollDown", IE_Pressed, this, &APhoenixCharacter::MoveDownInventorySlot);

	// Bind OnHit event -> LMB
	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &APhoenixCharacter::OnHit);
	PlayerInputComponent->BindAction("Hit", IE_Released, this, &APhoenixCharacter::EndHit);

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


// ===========================================================================
//		INVENTORY
// ===========================================================================
int32 APhoenixCharacter::GetCurrentInventorySlot()
{
	return CurrentInventorySlot;
}

bool APhoenixCharacter::AddItemToInventory(AItemBase* Item)
{
	if (Item)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);
		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
	}
	return false;
}

UTexture2D* APhoenixCharacter::GetThumbnailAtInventorySlot(uint8 Slot)
{
	if (Inventory[Slot])
	{
		return Inventory[Slot]->PickupThumbnail;
	}
	return nullptr;
}

void APhoenixCharacter::MoveUpInventorySlot()
{
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot + 1) % NUM_OF_INVENTORY_SLOTS);

	UpdateItem();
}

void APhoenixCharacter::MoveDownInventorySlot()
{
	if(CurrentInventorySlot == 0)
	{
		CurrentInventorySlot = 9;
		return;
	}
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot - 1) % NUM_OF_INVENTORY_SLOTS);

	UpdateItem();
}


// ===================================================================
//		MOVEMENT
// ===================================================================

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


// ===================================================================================
//		ITEMBASE & MOUSE BUTTON
// ===================================================================================
void APhoenixCharacter::UpdateItem()
{
	// If the current inventory slot is not empty then set the mesh to that item, otherwise make it null
	Inventory[CurrentInventorySlot] != NULL ? SKM_WieldedItem->SetSkeletalMesh(Inventory[CurrentInventorySlot]->WieldableMesh->SkeletalMesh) : SKM_WieldedItem->SetSkeletalMesh(NULL);
}

AItemBase* APhoenixCharacter::GetCurrentlyHeldItem()
{
	return Inventory[CurrentInventorySlot] != NULL ? Inventory[CurrentInventorySlot] : nullptr;
}

void APhoenixCharacter::Throw()
{
	// Get currently wielded item
	AItemBase* ItemToThrow = GetCurrentlyHeldItem();

	FHitResult LineTraceHit;

	FVector StartTrace = Camera->GetComponentLocation();

	FVector EndTrace = Camera->GetForwardVector() * Reach + StartTrace;

	FCollisionQueryParams FCQP;

	FCQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(LineTraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, FCQP);

	FVector DropLocation = EndTrace;
	
	if(LineTraceHit.GetActor())
	{
		DropLocation = LineTraceHit.ImpactPoint + 20.0f;
	}

	if(ItemToThrow)
	{
		UWorld* const World = GetWorld();

		if(World)
		{
			ItemToThrow->SetActorLocationAndRotation(DropLocation, FRotator::ZeroRotator);

			ItemToThrow->Hide(false);
		}
	}

	UpdateItem();
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

	// DEBUG
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
		UAnimInstance* AnimInstance = SKM_Player->GetAnimInstance();

		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(HitAnimation, 1.f);

			// DEBUG
			FString msg = (TEXT("Hit animation played"));

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, msg);
		}
	}
}

// =================================================================================
//		BLOCK
// =================================================================================
void APhoenixCharacter::CheckForBlock()
{
	FHitResult LinetraceHit;

	FVector StartTrace = Camera->GetComponentLocation();

	FVector EndTrace = (Camera->GetForwardVector() * Reach) + StartTrace;

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
	}
}

void APhoenixCharacter::BreakBlock()
{
	if (bIsBreakingBlock && CurrentBlock != nullptr && !CurrentBlock->IsPendingKill()) 
	{
		CurrentBlock->Break();
	}

}
