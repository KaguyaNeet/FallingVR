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

	bool GetWeaponOneHanded();

	FVector2D GetTwoAllowRange();
	bool isRunningFire();

	void SetElementType(EMyElement::Type NewType);

public:

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere)
		class UArrowComponent* MuzzleArrow;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow1;
	UPROPERTY(VisibleAnywhere)
		class UArrowComponent* HandArrow2;
	//霰弹枪一次射出的子弹数量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default Property")
		int ShotGunBulletNum = 15;

private:

	void BulletIsSingleProjectle();
	void BulletIsMultiProjectle();

	void AddRecoil();
	void SetRecoilRestore();

private:

	EMyWeapon::Type MyWeaponType = EMyWeapon::Pistol;
	EMyElement::Type MyElementType = EMyElement::None;

	UPROPERTY()
		class UDataTable* WeaponData;
	UPROPERTY()
		class UDataTable* ElementData;

	struct FWeaponType* MyDefaultProperty;
	struct FBulletParam* TempBulletParam;

	bool bRecoil = false;

	FTimerHandle RecoilHandle;

protected:

	virtual void Tick(float DeltaTime) override;
	
};


