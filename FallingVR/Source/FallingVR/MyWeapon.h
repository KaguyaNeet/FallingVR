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
		class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		float TwoHandMaxDistance = 40.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		float TwoHandMinDistance = 20.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		bool RunningFire = false;
	//霰弹枪一次射出的子弹数量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		int ShotGunBulletNum = 15;

private:
	void AddDamage(float DamageValue, const FHitResult& HitResult, AMyCharacterBase* Causer);

	void BulletIsProjectle();
	void BulletIsSingleTrace();
	void BulletIsMultiTrace();

private:

	EMyWeapon::Type MyWeaponType = EMyWeapon::Pistol;

	UParticleSystem* LineTraceBulletParticle = NULL;

protected:

	virtual void Tick(float DeltaTime) override;
	
};


