// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Block.h"
#include <Components/InputComponent.h>
#include "Wieldable.h"
#include "PhoenixCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class APhoenixCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;


public:

	APhoenixCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		float Food;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		float Water;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		float Experience;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		int CoinGold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		int CoinSilver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		int CoinCopper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ Variables")
		int Level;


public:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_WieldedItem;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* HitAnimation;

	// Get current inv slot for the HUD
	UFUNCTION(BlueprintPure, Category = "C++ HUD Fxn")
	int32 GetCurrentInventorySlot();

	// Adds an item to our inventory
	UFUNCTION(BlueprintCallable, Category = "C++ Inv Fxn")
	bool AddItemToInventory(AWieldable* Item);

	// Gets the thumbnail for a given item
	UFUNCTION(BlueprintPure, Category = "C++ Inv Fxn")
	UTexture2D* GetThumbnailAtInventorySlot(uint8 Slot);

	// Type and material of wielded item
	uint8 ToolType;

	uint8 MaterialType;

protected:
	
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


private:
	// # inventory slots
	const int32 NUM_OF_INVENTORY_SLOTS = 10;

	// Current inventory slot
	int32 CurrentInventorySlot;

	// Increment and decrement inventory slot
	void MoveUpInventorySlot();

	void MoveDownInventorySlot();

	// True if player has LMB down in front of a block
	bool bIsBreakingBlock;

	// Called when using RMB to place block or open stuff
	void OnInteract();

	void EndInteract();

	// Called when hitting with a tool
	void OnHit();

	void EndHit();

	// Plays the mining animation  (will try PickaxeSwingWall from ResourceGatheringAnimation Pack)
	void PlayHitAnim();

	// Check to see if a block is in front of the player
	void CheckForBlock();

	// Stores the block currently being looked at by the player
	ABlock* CurrentBlock;

	// Called when we want to break a block
	void BreakBlock();

	// Player character's reach
	float Reach;

	// Timer Handles
	FTimerHandle BlockBreakingHandle;
	FTimerHandle HitAnimHandle;

	//Inventory variable
	UPROPERTY(EditAnywhere)
	TArray<AWieldable*> Inventory;


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

