// Fill out your copyright notice in the Description page of Project Settings.
#include "MyEnums.h"
#pragma once

#include "CoreMinimal.h"
#include "MyItemBase.h"
#include "MyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGVR_API AMyWeapon : public AMyItemBase
{
	GENERATED_BODY()
	
public:

	AMyWeapon();

	virtual void ItemFunction() override;
	virtual void InitItem(class AMyCharacterBase* Owner_, EMyWeapon::Type NewType);

public:

	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* MuzzleArrow;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow1;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		class UParticleSystem* MuzzleParticle = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float FireDistance = 1000.f;
	//射击基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Property")
		float BaseDamageValue = 10.f;
	//是否为单手武器
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		bool One_handed = true;

private:
	void AddDamage(float DamageValue, FHitResult& HitResult, AMyCharacterBase* Causer);
	
};


