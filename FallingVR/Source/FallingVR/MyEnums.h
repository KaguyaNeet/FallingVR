// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "MyEnums.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGVR_API UMyEnums : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};

UENUM()
namespace EMyWeapon
{
	enum Type
	{
		Pistol,
		Rifle,
		MiniGun,
		Sniper,
		Shotgun
	};
}

UENUM()
namespace EMyHand
{
	enum Type
	{
		RightHand,
		LeftHand
	};
}

UENUM()
namespace EMyItem
{
	enum Type
	{
		None,
		FlashSphere,
		Shield,
		Shock
	};
}

UENUM()
namespace EMyElement
{
	enum Type
	{
		None,
		Fire,
		Ice,
		Explosion,
		Poison
	};
}