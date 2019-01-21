// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
// Reference: https://github.com/carlso70/UE4-Minecraft/tree/master/Source/Minecraft
// Reference (MC C++): https://www.youtube.com/playlist?list=PLXbYY5fO3b99aRm_Tmb3g2TH_8FxkKGyA
// Reference (spawn item with key press): https://www.youtube.com/watch?v=Z-mbe4k5Wa4
// Reference: https://unrealcpp.com/health-bar-ui-hud/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Block.h"
#include <Components/InputComponent.h>
#include "PhoenixCharacter.generated.h"


class UInputComponent;

UCLASS(config=Game)
class APhoenixCharacter : public ACharacter
{
		GENERATED_BODY()

		/** Location on gun mesh where projectiles should spawn. */
		UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class USceneComponent* FP_MuzzleLocation;

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

		/** Pawn mesh: 1st person view (arms; seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = "Components")
			class USkeletalMeshComponent* SKM_Player;

		/** Gun mesh: 1st person view (seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = "Components")
			class USkeletalMeshComponent* SKM_WieldedItem;


public:

		APhoenixCharacter();

protected:

		virtual void BeginPlay();

		virtual void Tick(float DeltaTime) override;


// ===============================================================================
//
//								PLAYER STATS (PUBLIC)
//
// ===============================================================================
public:

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float HealthMax;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float HealthCurrent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float HealthPercent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float HealthPrevious;

		FTimerHandle MemberTimerHandle;

		/** Get Health */
		UFUNCTION(BlueprintPure, Category = "C++ Functions")
			float GetHealthCurrent();

		/** Get Health Text */
		UFUNCTION(BlueprintPure, Category = "C++ Functions")
			FText GetHealthCurrentIntText();

// ===============================================================================
//
//								PLAYER DAMAGE (PUBLIC)
//
// ===============================================================================

		/* Damage Timer */
		UFUNCTION()
			void DamageTimer();

		/** Set Damage State */
		UFUNCTION()
			void SetDamageState();

		void ReceivePointDamage(float Damage, const class UDamageType * DamageType, FVector HitLocation, FVector HitNormal, class UPrimitiveComponent * HitComponent, FName BoneName, FVector ShotFromDirection, class AController * InstigatedBy, AActor * DamageCauser, const FHitResult & HitInfo);

		UFUNCTION(BlueprintCallable, Category = "C++ Functions")
			void UpdateHealthCurrent(float HealthChange);

// ===============================================================================
//
//								PLAYER SKILLS (PUBLIC)
//
// ===============================================================================

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int Strength = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int Dexterity = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int Intellect = 1;


		UFUNCTION(BlueprintCallable, Category = "C++ Skill Functions")
			int SkillDiceRollGrowthFast();

		UFUNCTION(BlueprintCallable, Category = "C++ Skill Functions")
			int SkillDiceRollGrowthMed();

		UFUNCTION(BlueprintCallable, Category = "C++ Skill Functions")
			int SkillDiceRollGrowthSlow();

	
// =================================================================================
//
//								PLAYER CURRENCY - $$$ (PUBLIC)
//
// =================================================================================	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int CoinGold = 0;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int CoinSilver = 0;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			int CoinCopper = 0;


// =================================================================================
//
//								PLAYER LEVELING (OUBLIC)
//
// =================================================================================
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float XpMax;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float XpCurrent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float XpPercent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
			float XpPrevious;

		/** Get Health */
		UFUNCTION(BlueprintPure, Category = "C++ Functions")
			float GetXpCurrent();

		/** Get Health Text */
		UFUNCTION(BlueprintPure, Category = "C++ Functions")
			FText GetXpCurrentIntText();

		
		void EarnXp(float XpEarned);

		UFUNCTION(BlueprintCallable, Category = "C++ Functions")
			void UpdateXpCurrent(float XpChange);


//////////////////////////////////////////////////////////////////////////////

		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
			float BaseTurnRate;

		/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
			float BaseLookUpRate;

		/** Gun muzzle's offset from the characters location */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
			FVector GunOffset;

		/** Projectile class to spawn */
		UPROPERTY(EditDefaultsOnly, Category=Projectile)
			TSubclassOf<class APhoenixProjectile> ProjectileClass;

		/** Sound to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
		class USoundBase* FireSound;

		/** AnimMontage to play each time we hit */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Gameplay)
		class UAnimMontage* HitAnimation;
//////////////////////////////////////////////////////////////////////////////


// ================================================================================
//
//							INVENTORY & HUD   (PUBLIC)
//
// ================================================================================
public:

		// Get current inv slot for the HUD
		UFUNCTION(BlueprintPure, Category = "C++ Inventory")
			int32 GetCurrentInventorySlot();

		// Adds an item to our inventory
		UFUNCTION(BlueprintCallable, Category = "C++ Inventory")
			bool AddItemToInventory(AItemBase* Item);

		// Gets the thumbnail for a given item
		UFUNCTION(BlueprintPure, Category = "C++ Inventory")
			UTexture2D* GetThumbnailAtInventorySlot(uint8 Slot);

		// Type and material of wielded item
		uint8 ToolType;

		uint8 MaterialType;



// ============================================================================
//
//									MOVEMENT
//
// ============================================================================
protected:

		// APawn interface
		virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

		/** Handles moving forward/backward */
		void MoveForward(float Val);

		/** Handles strafing movement, left and right */
		void MoveRight(float Val);

		/**
		 * Called via input to turn at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void TurnAtRate(float Rate);

		/**
		 * Called via input to turn look up/down at a given rate.
		 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
		 */
		void LookUpAtRate(float Rate);


// ============================================================================
//
//						INVENTORY FUNCTIONALITY  (PRIVATE)
//
// ============================================================================
private:

		//Inventory variable
		UPROPERTY(EditAnywhere)
			TArray<AItemBase*> Inventory;
	
		// Current ITEM inventory slot
		int32 CurrentInventorySlot;

		// # inventory slots
		const int32 NUM_OF_INVENTORY_SLOTS = 10;

		// Increment and decrement inventory slot
		void MoveUpInventorySlot();

		void MoveDownInventorySlot();


// ===============================================================================
//
//		HELD ITEM (ITEMBASE DERIVATIVES) & MOUSE BUTTON ITEM FUNCTIONALITY
//
// ===============================================================================
private:
	
		// Updates the wielded item
		void UpdateItem();

		// Gets currently wielded item
		AItemBase* GetCurrentlyHeldItem();

		// Throws currently wielded item
		void Throw();

		// Called when hitting with a tool
		void OnHit();
	
		void EndHit();

		// Plays the mining animation  (will try PickaxeSwingWall from ResourceGatheringAnimation Pack)
		void PlayHitAnim();

		// Timer Between tool swings
		FTimerHandle HitAnimHandle;


// ===============================================================================
//
//								BLOCK FUNCTIONALITY
//
// ===============================================================================
private:

		// True if player has LMB down in front of a block
		bool bIsBreakingBlock;

		// Check to see if a block is in front of the player
		void CheckForBlock();

		// Called when we want to break a block
		void BreakBlock();

		// Stores the block currently being looked at by the player
		UPROPERTY()
		class ABlock* CurrentBlock;

		// Player character's reach
		float Reach;

		// Timer Between Block breaks
		FTimerHandle BlockBreakingHandle;


//////////////////////////////////////////////////////////////////////////////////////
public:

		/** Returns PLayer Mesh subobject **/
		FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return SKM_Player; }

		/** Returns FirstPersonCameraComponent subobject **/
		FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return Camera; }
};
