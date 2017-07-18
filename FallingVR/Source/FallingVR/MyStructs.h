// Fill out your copyright notice in the Description page of Project Settings.
#include "MyEnums.h"
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyStructs.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGVR_API UMyStructs : public UDataTable
{
	GENERATED_BODY()
	
	
	
	
};

USTRUCT()
struct FWeaponType : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool One_Handed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireCD = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MuzzleOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MeshOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDamageValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* MuzzleParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* BulletParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RunningFire = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TwoHandMaxDistance = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TwoHandMinDistance = 20.f;

		
};
