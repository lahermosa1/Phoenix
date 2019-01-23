// Fill out your copyright notice in the Description page of Project Settings.
// Reference (overriding parent functions): https://answers.unrealengine.com/questions/284977/having-trouble-overriding-base-classs-function-fro.html

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.h"
#include "ItemBlock.generated.h"

UCLASS()
class PHOENIX_API AItemBlock : public AItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBlock();
};
